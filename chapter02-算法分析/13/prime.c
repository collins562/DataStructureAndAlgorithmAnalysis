#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#define TESTNUM 100

int  testnum(int *);
void testfn(int (*)(int), int *, int, char *);
double fntime(int (*)(int), int);
int isprime(int);

int main()
{
    int  i, (*fn)(int *, int);
    int  ts[] = { 1111111111, 4000000001, 0 };
    testfn(isprime, ts, testnum(ts), "O(N^0.5)");
}

int testnum(int *testArray)
{
    int *n;
    for (n = testArray; *n != 0; n++)
        ;
    return n - testArray;
}

void testfn(int (*fn)(int), int *ts, int tn, char *heading)
{
    int i;
    printf("        | %s\n", heading);
    for (i = 0; i < tn; i++)
        printf("%7u | %.8f\n", ts[i], fntime(fn, ts[i]));
}

double fntime(int (*fn)(int), int N)
{
    double i, start, end;
    start = clock();
    for (i = 0; i < TESTNUM; i++)
        fn(N);
    end = clock();
    return (end - start) / CLOCKS_PER_SEC / TESTNUM;
}

int isprime(int n)
{
    if (n <= 3)
        return 1;
    for (int i = 2; i <= sqrt(n); i++)
        if (n % i == 0)
            return 1;
    return 0;
}