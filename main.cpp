#include <cstdlib>
#include <iostream>

#include "MMSE.h"
#include "QuBLAS.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    int iter = 10000;
    double SNRdB = 20;
    if (argc >= 3)
    {
        iter = std::atoi(argv[1]);
        SNRdB = std::atof(argv[2]);
    }
    MMSE mmse;
    int error = 0;
    for (int loop = 0; loop < iter; loop++)
    {
        int info[TxAntNum * ModType];

        for (int i = 0; i < TxAntNum * ModType; i++)
        {
            info[i] = rand() % 2;
        }

        mmse.run(info, SNRdB);

        for (int i = 0; i < TxAntNum * ModType; i++)
        {
            if (mmse.info[i] != info[i])
            {
                error++;
            }
        }
    }

    std::cout << error << std::endl;
    return 0;
}