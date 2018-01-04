#include <stdio.h>
#include <stdlib.h>
#include "MinMaxHeap.h"

int main()
{
    int i, NumSize;
    int nums[] = { 6, 81, 87, 14, 17, 12, 28, 71, 25, 80, 20, 52, 78, 31, 42, 
                   31, 59, 16, 24, 79, 63, 18, 19, 32, 13, 15, 48 };
    PriorityQueue H;

    NumSize = sizeof(nums) / sizeof(nums[0]);

    H = Initialize(NumSize);
    for (i = 0; i < NumSize; i++)
        Insert(nums[i], H);
    PrintMinMaxHeap(H);

    for (i = 0; i < NumSize; i++)
        printf("%d ", DeleteMin(H));
    putchar('\n');
    PrintMinMaxHeap(H);
}