#include <stdio.h>
#include <stdlib.h>
#include "Avltree.h"

int main()
{
    int i;
    int nums[] = { 2, 1, 4, 5, 9, 3, 6, 7 };
    AvlTree T;
    T = NULL;

    for (i = 0; i < sizeof(nums)/sizeof(nums[0]); i++)
        T = Insert(nums[i], T);

    PrintAvlTree(T);
}