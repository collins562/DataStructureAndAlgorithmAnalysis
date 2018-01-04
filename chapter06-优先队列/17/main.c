#include <stdio.h>
#include <stdlib.h>
#include "LeftHeap.h"

int main()
{
    int i;
    PriorityQueue H;

    H = Initialize();
    for (i = 1; i < 16; i++)
        Insert(i, H);
    PrintLeftHeap(H);


}