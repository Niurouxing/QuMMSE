#pragma once

#include "Cons.h"
#include "QuBLAS.h"
#include "config.h"
#include "utils.h"

inline void GETRF(std::vector<std::vector<double>> &A, std::vector<int> &pivot)
{
    int n = A.size();

    pivot.resize(n, 0);

    for (int i = 0; i < n; ++i)
        pivot[i] = i;

    for (int k = 0; k < n; ++k)
    {
        double max = 0.0;
        int i_max = k;

        for (int i = k; i < n; ++i)
        {
            if (std::abs(A[i][k]) > max)
            {
                max = std::abs(A[i][k]);
                i_max = i;
            }
        }

        if (max == 0.0)
        {
            std::cerr << "Matrix is singular!" << std::endl;
            return;
        }

        std::swap(A[k], A[i_max]);
        std::swap(pivot[k], pivot[i_max]);

        for (int i = k + 1; i < n; ++i)
        {
            A[i][k] /= A[k][k];
            for (int j = k + 1; j < n; ++j)
            {
                A[i][j] -= A[i][k] * A[k][j];
            }
        }
    }
}

inline void GETRS(std::vector<std::vector<double>> &A, std::vector<int> &pivot, std::vector<double> &b)
{
    int n = A.size();
    std::vector<double> b_permuted(n);

    for (int i = 0; i < n; ++i)
    {
        b_permuted[i] = b[pivot[i]];
    }

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < i; ++j)
        {
            b_permuted[i] -= A[i][j] * b_permuted[j];
        }
    }

    for (int i = n - 1; i >= 0; --i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            b_permuted[i] -= A[i][j] * b_permuted[j];
        }
        b_permuted[i] /= A[i][i];
    }

    for (int i = 0; i < n; ++i)
    {
        b[i] = b_permuted[i];
    }
}

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

        HtY.display("result");
    }
};
