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
int exponentiation(int, int);

int main()
{
    printf("%d\n", exponentiation(2, 10));
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

int exponentiation(int base, int power)
{
    if (base == 0)
        return 0;
    else if (power == 0)
        return 1;

    int res = 1;
    while (power > 0) {
        if (power % 2 == 1) {
            res *= base;
            power--;
        } else {
            base *= base;
            power /= 2;
        }
    }
    return res;
}