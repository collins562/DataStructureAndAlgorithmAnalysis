#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define TESTNUM 100

int  testnum(int *);
double fntime(int);
unsigned long ulrand(void);
unsigned long RandInt(int, int);
int *RandArray(int *, int N);

int main()
{
    int  i, j;
    int  Ts[] = { 100, 1000, 10000, 100000, 1000000, 0 };
    char *Tobjs[] = { "  QSort  " };

    printf("|    N    | %s |\n", "  QSort  ");
    for (i = 0; i < testnum(Ts); i++)
        printf("| %7d |  %.5f  |\n", Ts[i], fntime(Ts[i]));
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
        A[i] = RandInt(0, 1);
    return A;
}

int cmp(const void *a, const void *b)
{
    return (*(int*)a - *(int*)b);
}

double fntime(int N)
{
    int *A;
    double i, Start, End, TotalTime = 0;

    if ((A = malloc(sizeof(int) * N)) == NULL)
        fprintf(stderr, "Out of space\n");
    for (i = 0; i < TESTNUM; i++) {
        A = RandArray(A, N);
        Start = clock();
        qsort(A, N, sizeof(int), cmp);
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
