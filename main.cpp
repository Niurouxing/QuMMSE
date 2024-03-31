#include <iostream>
#include <cstdlib>

#include "MMSE.h"
#include "QuBLAS.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <iter> <SNRdB>" << std::endl;
        return 1;
    }

    int iter = std::atoi(argv[1]);
    double SNRdB = std::atof(argv[2]);

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

    std::cout <<error << std::endl;
    return 0;
}