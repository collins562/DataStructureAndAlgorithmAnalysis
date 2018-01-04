#include <stdio.h>
#include <stdlib.h>
#include "SkewHeap.h"

int main()
{
    int i;
    PriorityQueue H;

    H = Initialize();
    for (i = 1; i < 16; i++)
        Insert(i, H);
    PrintSkewHeap(H);

    H = Initialize();
    for (i = 15; i > 0; i--)
        Insert(i, H);
    PrintSkewHeap(H);
}