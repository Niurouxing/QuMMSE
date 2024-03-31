#include <iostream>

#include "QuBLAS.h"
#include "utils.h"
#include "MMSE.h"


int main()
{
    MMSE mmse;

    int temp[TxAntNum * ModType];

    for (int i = 0; i < TxAntNum * ModType; i++)
    {
        temp[i] = i % 2;
    }

    mmse.run(temp,30);
    




}