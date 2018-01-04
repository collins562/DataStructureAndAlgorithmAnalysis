#include <stdio.h>
#include <stdlib.h>
#include "Splaytree.h"

void PrintST(SplayTree T);

int main()
{
    int i;
    int nums[] = { 10, 11, 12, 13, 4, 2, 1, 3, 6, 5, 8, 7, 9 };
    SplayTree T;
    T = NULL;

    for (i = 0; i < sizeof(nums)/sizeof(nums[0]); i++)
        T = Insert(nums[i], T);

    PrintST(T);

    T = SplayFind(3, T);
    PrintST(T);


    T = SplayFind(9, T);
    PrintST(T);

    T = SplayFind(1, T);
    PrintST(T);

    T = SplayFind(5, T);
    PrintST(T);
}

void PrintST(SplayTree T)
{
    PrintSplayTree(T);
    putchar('\n');
}