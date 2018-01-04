#include <stdio.h>
#include "Sort.h"

int main()
{
    int i, NumSize;
    int Nums[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1 };

    NumSize = sizeof(Nums) / sizeof(Nums[0]);
    PrintArray(Nums, NumSize);
    ShellSort(Nums, NumSize);
    PrintArray(Nums, NumSize);
}