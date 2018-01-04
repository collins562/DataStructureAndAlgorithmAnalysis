#include <stdio.h>
#include "tree.h"

int main()
{
    int i;
    SearchTree T = NULL;

    for (i = 0; i < 20; i++)
        T = Insert(i, T);

    PrintBetween(T, 5, 15);
}