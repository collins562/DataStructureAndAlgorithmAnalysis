#include <stdio.h>
#include "Sort.h"

int main()
{
    int i, NumSize;
    int Nums[] = { 3, 1, 4, 1, 5, 9, 2, 6 };

    NumSize = sizeof(Nums) / sizeof(Nums[0]);
    PrintArray(Nums, NumSize);
    MergeSort(Nums, NumSize);
    PrintArray(Nums, NumSize);
}