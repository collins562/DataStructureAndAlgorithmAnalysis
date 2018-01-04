#include <stdio.h>
#include "tree.h"

int main()
{
    int i;
    SearchTree T = NULL, T2 = NULL;

    for (i = 0; i < 20; i++)
        T = Insert(i, T);
    for (i = 3; i < 15; i++)
        T2 = Insert(i, T2);

    printf("%d\n", IsSimiliar(T, T));
    printf("%d\n", IsSimiliar(T, T2));
}