#include <stdio.h>
#include <stdlib.h>
#include "LeftHeap.h"

int main()
{
    int i, NumSize1, NumSize2;
    int nums1[] = { 2, 11, 5, 12, 17, 8, 18, 15 };
    int nums2[] = { 4, 9, 6, 18, 10, 11, 31, 21 };
    PriorityQueue H1, H2;

    NumSize1 = sizeof(nums1) / sizeof(nums1[0]);
    NumSize2 = sizeof(nums2) / sizeof(nums2[0]);

    H1 = H2 = Initialize();
    for (i = 0; i < NumSize1; i++)
        Insert(nums1[i], H1);
    for (i = 0; i < NumSize2; i++)
        Insert(nums2[i], H2);
    PrintLeftHeap(H1);
    PrintLeftHeap(H2);

    PrintLeftHeap(Merge(H1, H2));
    for (i = 0; i < NumSize1 + NumSize2; i++) {
        printf("%d ", FindMin(H1));
        DeleteMin(H1);
    }
}