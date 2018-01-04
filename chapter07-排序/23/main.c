#include <stdio.h>
#include "Sort.h"

#define Cutoff 3

int main()
{
    int i, NumSize;
    int Nums[] = { 3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5 };

    NumSize = sizeof(Nums) / sizeof(Nums[0]);
    for (i = 0; i < NumSize; i++)
        printf("%d ", QuickSelect(Nums, i, NumSize));
    putchar('\n');
}

