#include <stdio.h>
#include "Sort.h"

int main()
{
    int i, NumSize;
    int Nums[] = { 142, 543, 123, 65, 453, 879, 572, 434, 111, 242, 811, 102 };

    NumSize = sizeof(Nums) / sizeof(Nums[0]);
    PrintArray(Nums, NumSize);
    HeapSort(Nums, NumSize);
    PrintArray(Nums, NumSize);
}