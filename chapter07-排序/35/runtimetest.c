#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "ShellSort.h"

#define TESTNUM 100

int  testnum(int *);
double fntime(void (*)(ElementType *, int), int);
unsigned long ulrand(void);
unsigned long RandInt(int, int);
int *RandArray(int *, int N);

int main()
{
    int  i, j, TestSize;
    int  Ts[] = { 100, 1000, 10000, 100000, 1000000, 0 };
    char *Tobjs[] = { 
        "  Shell  ", " Hibbard ", "  Knuth  ", " Gonnet  ", "Sedgewick" 
    };
    void (*fns[])(ElementType*, int) = { 
        ShellSort1, ShellSort2, ShellSort3, ShellSort4, ShellSort5 
    };

    TestSize = sizeof(fns) / sizeof(fns[0]);
    printf("|    N    |");
    for (i = 0; i < TestSize; i++)
        printf(" %s |", Tobjs[i]);
    putchar('\n');
    for (i = 0; i < testnum(Ts); i++) {
        printf("| %7d |", Ts[i]);
        for (j = 0; j < TestSize; j++)
            printf("  %.5f  |", fntime(fns[j], Ts[i]));
        printf("\n");
    }
}

int testnum(int *testArray)
{
    int *n;
    for (n = testArray; *n != 0; n++)
        ;
    return n - testArray;
}

int *RandArray(int A[], int N)
{
    int i, Tmp, Index;

    for (i = 0; i < N; i++)
        A[i] = i;
    for (i = 1; i < N; i++) {
        Index = RandInt(0, i);
        Tmp = A[i];
        A[i] = A[Index];
        A[Index] = Tmp;
    }
    return A;
}

double fntime(void (*fn)(ElementType *, int), int N)
{
    int *A;
    double i, Start, End, TotalTime = 0;

    if ((A = malloc(sizeof(int) * N)) == NULL)
        fprintf(stderr, "Out of space\n");
    for (i = 0; i < TESTNUM; i++) {
        A = RandArray(A, N);
        Start = clock();
        fn(A, N);
        End = clock();
        TotalTime += End - Start;
    }
    free(A);
    return TotalTime / CLOCKS_PER_SEC / TESTNUM;
}

/* ulrand: return a unsigned long random num                    */
unsigned long ulrand(void) {
    return ((((unsigned long) rand()<<24) & 0xFF000000ul) |
            (((unsigned long) rand()<<12) & 0x00FFF000ul) |
            (((unsigned long) rand()    ) & 0x00000FFFul));
}

unsigned long RandInt(int lower, int upper)
{
    return ulrand() % (upper-lower+1) + lower;
}
