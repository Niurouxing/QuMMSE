#include <iostream>

#include "QuBLAS.h"


void GETRF(std::vector<std::vector<double>> &A, std::vector<int> &pivot)
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

void GETRS(std::vector<std::vector<double>> &A, std::vector<int> &pivot, std::vector<double> &b)
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

int main(int, char**){
     std::cout << "----------------------------------" << std::endl;

    std::cout << "矩阵测试" << std::endl;

    using MAT_a1_t = apFixed<intBits<5>, fracBits<3>, isSigned<true>>;

    using MAT_a2_t = apFixed<intBits<5>, fracBits<4>, isSigned<true>>;

    using MAT_res_t = apFixed<intBits<13>, fracBits<2>, isSigned<true>>;

    apFixedMat<MAT_a1_t, 3, 5> MAT_a1 = {1.125, 2.125, 3.125, 4.125, 5.125,
                                         6.125, 7.125, 8.125, 9.125, 10.125,
                                         11.125, 12.125, 13.125, 14.125, 15.125};

    apFixedMat<MAT_a2_t, 5, 4> MAT_a2 = {1.25, 2.25, 3.25, 4.25,
                                         5.25, 6.25, 7.25, 8.25,
                                         9.25, 10.25, 11.25, 12.25,
                                         13.25, 14.25, 15.25, 16.25,
                                         17.25, 18.25, 19.25, 20.25};

    apFixedMat<MAT_res_t, 3, 4> MAT_res;

    // version 1 directly pass in the template parameters
    using addArgs = QgemulAddArgs<OfMode<WRP::TCPL>, intBits<17>, fracBits<13>>;

    // version 2 pass in a apFixed type
    using mulType = apFixed<intBits<13>, fracBits<7>>;
    using mulArgs = QgemulMulArgs<mulType>;

    Qgemul<addArgs, mulArgs>(MAT_res, MAT_a1, MAT_a2);

    MAT_res.display("MAT_res = MAT_a1 * MAT_a2");

    std::cout << "----------------------------------" << std::endl;

    std::cout << "负数位宽测试" << std::endl;

    using NEG_a1_t = apFixed<intBits<8>, fracBits<-2>>;

    NEG_a1_t NEG_a1 = 100;

    NEG_a1.display("NEG_a1");

    using NEG_a2_t = apFixed<intBits<-2>, fracBits<8>>;

    NEG_a2_t NEG_a2 = 0.025;

    NEG_a2.display("NEG_a2");

    std::cout << std::endl;

    std::cout << "----------------------------------" << std::endl;

    std::cout << "负数小数位量化测试" << std::endl;

    using NEG_FRAC_res_t = apFixed<intBits<-2>, fracBits<9>>;

    NEG_FRAC_res_t NEG_FRAC_res;

    NEG_FRAC_res = NEG_a2;

    NEG_FRAC_res.display("NEG_FRAC_res = NEG_a2");

    std::cout << std::endl;

    std::cout << "----------------------------------" << std::endl;

    std::cout << "矩阵求逆测试" << std::endl;

    // normal version
    std::vector<std::vector<double>> Anormal = {
        {1.1, 2.2, 3.3},
        {2, 1, 1},
        {3.1, 0, 1.1}};
    std::vector<double> bnormal = {14, 4, 8};

    std::vector<int> pivot;
    GETRF(Anormal, pivot);
    GETRS(Anormal, pivot, bnormal);

    std::cout << "GETRF result:" << std::endl;
    for (int i = 0; i < Anormal.size(); i++)
    {
        for (int j = 0; j < Anormal[i].size(); j++)
        {
            std::cout << Anormal[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    std::cout << "GETRS result:" << std::endl;
    for (int i = 0; i < bnormal.size(); i++)
    {
        std::cout << bnormal[i] << " ";
    }

    std::cout << std::endl;
    std::cout << std::endl;

    // QuBLAS version
    using MAT_INV_a1_t = apFixed<intBits<5>, fracBits<5>, isSigned<true>>;
    using MAT_INV_b1_t = apFixed<intBits<5>, fracBits<6>, isSigned<true>>;

    using interiorType1 = apFixed<intBits<11>, fracBits<6>, isSigned<true>, QuMode<RND::INF>>;

    using interiorType2 = apFixed<intBits<11>, fracBits<7>, isSigned<true>, QuMode<RND::ZERO>>;

    using interiorType3 = apFixed<intBits<11>, fracBits<4>, isSigned<true>, QuMode<TRN::TCPL>>;

    std::array<size_t, 3> ipiv;

    apFixedMat<MAT_INV_a1_t, 3, 3> MAT_INV_a1 = {1.1, 2.2, 3.3,
                                                 2, 1, 1,
                                                 3.1, 0, 1.1};

    apFixedVec<MAT_INV_b1_t, 3> MAT_INV_b1 = {14, 4, 8};

    Qgetrf<QgetrfMulArgs<intBits<11>, fracBits<6>, isSigned<true>, QuMode<RND::INF>>,
           QgetrfDivArgs<interiorType2>,
           QgetrfSubArgs<intBits<11>, fracBits<4>, isSigned<true>, QuMode<TRN::TCPL>>>(MAT_INV_a1, ipiv);

    std::cout << "GETRF result:" << std::endl;
    MAT_INV_a1.display("MAT_INV_a1");

    Qgetrs<QgetrfSubArgs<interiorType3>, QgetrfMulArgs<interiorType1>>(MAT_INV_a1, ipiv, MAT_INV_b1);

    std::cout << "GETRS result:" << std::endl;
    MAT_INV_b1.display("MAT_INV_b1");
}
