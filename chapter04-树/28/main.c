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
    putchar('\n');
    printf("Nodes: %d\n", CountNodes(T));
    printf("Leaves: %d\n", CountLeaves(T));
    printf("Fullnodes: %d\n", CountFullNodes(T));
}