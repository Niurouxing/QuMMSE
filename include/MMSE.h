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
    Qu<dim<2 * RxAntNum>, RxSymbol_t> RxSymbols;
    Qu<dim<2 * RxAntNum, 2 * TxAntNum>, H_t> H;
    Qu<intBits<0>, fracBits<10>> Nv;

    Qu<dim<2 * TxAntNum, 2 * TxAntNum>, L_t> L;
    Qu<dim<2 * TxAntNum>, b_t> b;

    std::array<double, TxAntNum * 2> TxSymbolsNoQuant;
    std::array<double, RxAntNum * 2> RxSymbolsNoQuant;
    std::array<std::array<double, TxAntNum * 2>, RxAntNum * 2> HNoQuant;

    std::array<int, TxAntNum * ModType> info;

    MMSE() = default;

    void run(int *infoBits, double SNRdB)
    {
        static double NvNoQuant = TxAntNum * RxAntNum / (std::pow(10, SNRdB / 10) * ModType * TxAntNum);
        Nv = NvNoQuant;
        int ConSize = 1 << (ModType / 2);
        int bitLength = ModType / 2;
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
        }

        // generate H
        double randomTemp;
        for (int r = 0; r < RxAntNum; r++)
        {
            for (int t = 0; t < TxAntNum; t++)
            {
                randomTemp = randomGaussian_divSqrt2();
                H[r, t] = randomTemp;
                H[r + RxAntNum, t + TxAntNum] = randomTemp;
                HNoQuant[r][t] = randomTemp;
                HNoQuant[r + RxAntNum][t + TxAntNum] = randomTemp;

                randomTemp = randomGaussian_divSqrt2();
                H[r, t + TxAntNum] = randomTemp;
                H[r + RxAntNum, t] = -randomTemp;
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
        Qgemul<QgemulAddArgs<QgemulAddList>, QgemulMulArgs<QgemulMul_t>, QgemulTransposedA<true>>(L, H, H);

        Qgemv<QgemvAddArgs<QgemvAddList>, QgemvMulArgs<QgemvMul_t>, QgemvTransposedA<true>>(b, H, RxSymbols);

        for (int i = 0; i < 2 * TxAntNum; i++)
        {
            L[i, i] = L[i, i] + Nv;
        }

        Qpotrf<QpotrfMulArgs<QpotrfMul_t>,
               QpotrfSubArgs<QpotrfSub_t>,
               QpotrfDivArgs<QpotrfDiv_t>>(L);

        Qpotrs<QpotrsForwardMulArgs<QpotrsForwardMul_t>,
               QpotrsForwardSubArgs<QpotrsForwardSub_t>,
               QpotrsForwardDivArgs<QpotrsForwardDiv_t>,
               QpotrsBackwardMulArgs<QpotrsBackwardMul_t>, QpotrsBackwardSubArgs<QpotrsBackwardSub_t>,
               QpotrsBackwardDivArgs<QpotrsBackwardDiv_t>>(L, b);

        auto res = b.toDouble();

        const double *Cons;
        const int *bitCons;
        if constexpr (ModType == 2)
        {
            Cons = realConsMod2;
            bitCons = realBitConsMod2;
        }
        else if constexpr (ModType == 4)
        {
            Cons = realConsMod4;
            bitCons = realBitConsMod4;
        }
        else if constexpr (ModType == 6)
        {
            Cons = realConsMod6;
            bitCons = realBitConsMod6;
        }
        else if constexpr (ModType == 8)
        {
            Cons = realConsMod8;
            bitCons = realBitConsMod8;
        }
        else
        {
            // static_assert(false, "ModType not supported");
            throw std::runtime_error("ModType not supported");
        }
        for (int i = 0; i < TxAntNum * 2; i++)
        {
            double minDistance = 100000000;
            int minIndex = 0;

            for (int j = 0; j < ConSize; j++)
            {
                double distance = 0;
                distance = std::abs(res[i] - Cons[j]);

                if (distance < minDistance)
                {
                    minDistance = distance;
                    minIndex = j;
                }
            }

            for (int j = 0; j < bitLength; j++)
            {
                this->info[i * bitLength + j] = bitCons[minIndex * bitLength + j];
            }
        }
    }
};
