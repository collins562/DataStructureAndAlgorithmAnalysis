#include <stdio.h>
#include <stdlib.h>
#include "BinHeap.h"

int main()
{
    int i, NumSize;
    int nums[] = { 10, 12, 1, 14, 6, 5, 8, 15, 3, 9, 7, 4, 11, 13, 2 };
    PriorityQueue H;

    NumSize = sizeof(nums) / sizeof(nums[0]);

    H = Initialize(NumSize-1);
    for (i = 0; i < NumSize; i++)
        Insert(nums[i], H);
    PrintBinHeap(H);
    IncreaseKey(1, 20, H);
    PrintBinHeap(H);
    Delete(8, H);
    PrintBinHeap(H);

}