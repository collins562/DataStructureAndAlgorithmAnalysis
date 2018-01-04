#include <stdio.h>
#include <stdlib.h>
#include "BinQueue.h"

int main()
{
    int i, NumSize1, NumSize2;
    int nums1[] = { 21, 12, 24, 65, 26, 14, 16, 18, 51, 23, 65, 24, 13 };
    int nums2[] = { 11, 2, 55, 29, 15, 18, 4 };
    BinQueue H1, H2;

    NumSize1 = sizeof(nums1) / sizeof(nums1[0]);
    NumSize2 = sizeof(nums2) / sizeof(nums2[0]);

    H1 = Initialize();
    H2 = Initialize();
    for (i = 0; i < NumSize1; i++)
        Insert(nums1[i], H1);
    for (i = 0; i < NumSize2; i++)
        Insert(nums2[i], H2);
    PrintBinQueue(H1);
    PrintBinQueue(H2);

    PrintBinQueue(Merge(H1, H1));
    PrintBinQueue(Merge(H2, H2));
}