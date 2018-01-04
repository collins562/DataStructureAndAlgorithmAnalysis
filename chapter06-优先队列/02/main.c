#include <stdio.h>
#include <stdlib.h>
#include "BinHeap.h"

int main()
{
    int i, NumSize;
    int nums[] = { -1, 10, 12, 1, 14, 6, 5, 8, 15, 3, 9, 7, 4, 11, 13, 2 };
    PriorityQueue H;

    NumSize = sizeof(nums) / sizeof(nums[0]);

    H = Initialize(NumSize-1);
    
    // 6.2a
    for (i = 1; i < NumSize; i++)
        Insert(nums[i], H);
    PrintBinHeap(H);

    // 6.2b
    H->Elements = nums;
    BuildHeap(H);
    PrintBinHeap(H);
}