#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "Avltree.h"

#define TESTNUM     1000
#define TREESIZE    2000
#define FINDTIMES   20000

double fntime(void (*fn)(AvlTree), AvlTree T);
void test_find(AvlTree T);

int main()
{
    int i;
    AvlTree T = NULL;

    srand(time(NULL));
    for (i = 0; i < TREESIZE; i++)
        T = Insert(rand(), T);

    printf("Random Find Test Runtime: %.6f", fntime(test_find, T));
}

double fntime(void (*fn)(AvlTree), AvlTree T)
{
    double i, start, end;
    start = clock();
    for (i = 0; i < TESTNUM; i++)
        fn(T);
    end = clock();
    return (end - start) / TESTNUM;
}

/* test for AvlTree Find */
void test_find(AvlTree T)
{
    int i;

    srand(time(NULL));
    for (i = 0; i < FINDTIMES; i++)
        Find(rand(), T);
}

