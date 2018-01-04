#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Splaytree.h"

int main()
{
    int i;
    SplayTree T;
    T = Initialize();
    srand(time(NULL));

    for (i = 1; i <= 32; i++)
        T = Insert(rand() % 100, T);
    PrintSplayTree(T);
    for (i = 1; i <= 9; i++) {
        T = Find(i, T);
        printf("Approximate Element: %d.\n", Retrieve(T));
    }

    T = FindMin(T);
    printf("Minimum Element: %d.\n", Retrieve(T));

    T = FindMax(T);
    printf("Maximum Element: %d.\n", Retrieve(T));
    
    for (i = 10; i <= 20; i++)
        T = Delete(i, T);
    PrintSplayTree(T);
}