#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "RedBlackTree.h"

int main()
{
    int i;
    RedBlackTree T, SucceedT;
    Position P;
    T = Initialize();
    srand(time(NULL));

    for (i = 1; i <= 12; i++)
        Insert(rand() % 100, T);
    PrintRedBlackTree(T);

    P = FindMin(T);
    printf("Minimum Element: %d.\n", Retrieve(P));

    P = FindMax(T);
    printf("Maximum Element: %d.\n", Retrieve(P));
    
    for (i = 1; i < 5; i++) {
        printf("Remove %2d: ", Retrieve(Top(T)));
        Remove(Retrieve(Top(T)), T);
        PrintRedBlackTree(T);
    }
}