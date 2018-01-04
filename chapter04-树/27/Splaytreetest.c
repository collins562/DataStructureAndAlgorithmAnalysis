#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "Splaytree.h"

#define TESTNUM     1
#define TREESIZE    2000
#define FINDTIMES   20000

double fntime(SplayTree (*fn)(SplayTree), SplayTree T);
SplayTree test_find(SplayTree T);

int main()
{
    int i;
    SplayTree T = NULL;

    srand(time(NULL));
    for (i = 0; i < TREESIZE; i++)
        T = Insert(rand(), T);

    printf("Random Find Test Runtime: %.6f", fntime(test_find, T));
}

double fntime(SplayTree (*fn)(SplayTree), SplayTree T)
{
    double i, start, end;
    start = clock();
    for (i = 0; i < TESTNUM; i++)
        T = fn(T);
    end = clock();
    return (end - start) / TESTNUM;
}

/* test for SplayTree Find */
SplayTree test_find(SplayTree T)
{
    int i;
    Position P;

    srand(time(NULL));
    for (i = 0; i < FINDTIMES; i++)
        if ((P = SplayFind(rand(), T)) != NULL)
            T = P;
    return T;
}

