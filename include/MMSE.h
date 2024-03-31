#pragma once

#include "Cons.h"
#include "QuBLAS.h"
#include "config.h"
#include "utils.h"

static constexpr int TxAntNum = 16;
static constexpr int RxAntNum = 32;
static constexpr int ModType = 4;

class MMSE
{
public:
    apFixedVec<TxSymbol_t, 2 * TxAntNum> TxSymbols;
    apFixedVec<RxSymbol_t, 2 * RxAntNum> RxSymbols;
    apFixedMat<H_t, 2 * RxAntNum, 2 * TxAntNum> H;
    Nv_t Nv;

    apFixedMat<HtH_t, 2 * TxAntNum, 2 * TxAntNum> HtH;
    apFixedVec<HtY_t, 2 * TxAntNum> HtY;

    std::array<double, TxAntNum * 2> TxSymbolsNoQuant;
    std::array<double, RxAntNum * 2> RxSymbolsNoQuant;
    std::array<std::array<double, TxAntNum * 2>, RxAntNum * 2> HNoQuant;
    std::array<size_t, TxAntNum * 2> pivot;

    std::array<int, TxAntNum * ModType> info;

    MMSE() = default;

    void run(int *infoBits, double SNRdB)
    {
        static double NvNoQuant = TxAntNum * RxAntNum / (std::pow(10, SNRdB / 10) * ModType * TxAntNum);
        Nv = NvNoQuant;
        for (int i = 0; i < TxAntNum * 2; i++)
        {
            // each symbol contains Modtype/2 bits, read the bits to get the index
            int index = 0;
            for (int j = 0; j < ModType / 2; j++)
            {
                index = index * 2 + infoBits[i * ModType / 2 + j];
            }
            if constexpr (ModType == 2)
            {
                TxSymbolsNoQuant[i] = realConsMod2[index];
            }
            else if constexpr (ModType == 4)
            {
                TxSymbolsNoQuant[i] = realConsMod4[index];
            }
            else if constexpr (ModType == 6)
            {
                TxSymbolsNoQuant[i] = realConsMod6[index];
            }
            else if constexpr (ModType == 8)
            {
                TxSymbolsNoQuant[i] = realConsMod8[index];
            }
            else
            {
                // static_assert(false, "ModType not supported");
                throw std::runtime_error("ModType not supported");
            }
            TxSymbols[i] = TxSymbolsNoQuant[i];
        }

        // generate H
        double randomTemp;
        for (int r = 0; r < RxAntNum; r++)
        {
            for (int t = 0; t < TxAntNum; t++)
            {
                randomTemp = randomGaussian_divSqrt2();
                H[r][t] = randomTemp;
                H[r + RxAntNum][t + TxAntNum] = randomTemp;
                HNoQuant[r][t] = randomTemp;
                HNoQuant[r + RxAntNum][t + TxAntNum] = randomTemp;

                randomTemp = randomGaussian_divSqrt2();
                H[r][t + TxAntNum] = randomTemp;
                H[r + RxAntNum][t] = -randomTemp;
                HNoQuant[r][t + TxAntNum] = randomTemp;
                HNoQuant[r + RxAntNum][t] = -randomTemp;
            }
        }

        // generate random RxSymbols
        for (int r = 0; r < RxAntNum * 2; r++)
        {
            RxSymbolsNoQuant[r] = 0;
            for (int t = 0; t < TxAntNum * 2; t++)
            {
                RxSymbolsNoQuant[r] += HNoQuant[r][t] * TxSymbolsNoQuant[t];
            }

            RxSymbolsNoQuant[r] += randomGaussian() * std::sqrt(NvNoQuant / 2);

            RxSymbols[r] = RxSymbolsNoQuant[r];
        }

        // MMSE
        Qgemul<QgemulAddArgs<QgemulAdd_t>, QgemulMulArgs<QgemulMul_t>>(HtH, H);

        Qgemv<QgemvAddArgs<QgemvAdd_t>, QgemvMulArgs<QgemvMul_t>, QgemvTransposedA<true>>(HtY, H, RxSymbols);

        for (int i = 0; i < 2 * TxAntNum; i++)
        {
            HtH[i][i] = HtH[i][i] + Nv;
        }

        Qgetrf<QgetrfDivArgs<QgetrfDiv_t>,
               QgetrfSubArgs<QgetrfSub_t>,
               QgetrfMulArgs<QgetrfMul_t>>(HtH, pivot);

        Qgetrs<QgetrsDivArgs<QgetrsDiv_t>,
               QgetrsSubArgs<QgetrsSub_t>,
               QgetrsMulArgs<QgetrsMul_t>>(HtH, pivot, HtY);

        auto res = HtY.output();
        for (int i = 0; i < 2 * TxAntNum; i++)
        {
            for (int j = 0; j < ModType / 2; j++)
            {
                int index = 0;
                double minDist = 1e9;
                const double* realConsMod = nullptr;
                const int* realBitConsMod = nullptr;

                if constexpr (ModType == 2)
                {
                    realConsMod = realConsMod2;
                    realBitConsMod = realBitConsMod2;
                }
                else if constexpr (ModType == 4)
                {
                    realConsMod = realConsMod4;
                    realBitConsMod = realBitConsMod4;
                }
                else if constexpr (ModType == 6)
                {
                    realConsMod = realConsMod6;
                    realBitConsMod = realBitConsMod6;
                }
                else if constexpr (ModType == 8)
                {
                    realConsMod = realConsMod8;
                    realBitConsMod = realBitConsMod8;
                }
                else
                {
                    throw std::runtime_error("ModType not supported");
                }

                for (int k = 0; k < ModType / 2; k++)
                {
                    double dist = std::abs(res[i] - realConsMod[k]);
                    if (dist < minDist)
                    {
                        minDist = dist;
                        index = k;
                    }
                }

                for (int k = 0; k < ModType / 2; k++)
                {
                    info[i * ModType / 2 + k] = realBitConsMod[index * ModType / 2 + k];
                }
            }
        }
    }
};
