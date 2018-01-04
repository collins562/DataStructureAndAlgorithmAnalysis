#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define TESTNUM 10

int  testnum(int *);
void testfn(void (*)(int *, int), int *, int, char *);
double fntime(void (*)(int *, int), int *, int);
unsigned long ulrand(void);
unsigned long RandInt(int, int);
void rpermutation1(int *, int);
void rpermutation2(int *, int);
void rpermutation3(int *, int);
void swap(int *, int *);

int main()
{
    int  i, (*fn)(int *, int);
    int  ts1[] = { 250, 500, 1000, 2000, 0 };
    int  ts2[] = { 2500, 5000, 10000, 20000, 40000, 80000, 0 };
    int  ts3[] = { 10000, 20000, 40000, 80000, 160000, 320000, 0 };
    int  *ts[] = { ts1, ts2, ts3 };
    char *Os[] = { ">O(N^2)", "O(N)", "O(N)" };
    void (*fns[])(int *, int) = {rpermutation1, rpermutation2, rpermutation3};

    for (i = 0; i < sizeof(fns) / sizeof(fns[0]); i++)
        testfn(fns[i], ts[i], testnum(ts[i]), Os[i]);
}

int testnum(int *testArray)
{
    int *n;
    for (n = testArray; *n != 0; n++)
        ;
    return n - testArray;
}

void testfn(void (*fn)(int *, int), int *ts, int tn, char *heading)
{
    int i;
    printf("        | %s\n", heading);
    for (i = 0; i < tn; i++) {
        int A[ts[i]];
        printf("%7d | %.8f\n", ts[i], fntime(fn, A, ts[i]));
    }
}

double fntime(void (*fn)(int *, int), int *A, int N)
{
    double i, start, end;
    start = clock();
    for (i = 0; i < TESTNUM; i++)
        fn(A, N);
    end = clock();
    return (end - start) / CLOCKS_PER_SEC / TESTNUM;
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

void rpermutation1(int *A, int n)
{
    int i, j, rn;
    for (i = 0; i < n; ) {
        rn = RandInt(0, n) + 1;
        for (j = 0; j < i && A[j] != rn; j++)
            ;
        if (j == i)
            A[i++] = rn;
    }
}

void rpermutation2(int *A, int n)
{
    int i, rn, Used[n+1];   /* keep record of 1 ~ n used        */
    for (i = 0; i < n+1; i++)
        Used[i] = 0;
    for (i = 0; i < n; ) {
        rn = RandInt(0, n) + 1;
        if (Used[rn] == 0) {
            A[i++] = rn;
            Used[rn] = 1;
        }
    }
}

void rpermutation3(int *A, int n)
{
    int i, rn;
    while (i < n)
        A[i]  = ++i;
    for (i = 0; i < n; i++)
        swap(&A[i], &A[RandInt(0, i)]);
}

void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}