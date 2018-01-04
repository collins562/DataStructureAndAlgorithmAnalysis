#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define TESTNUM 1

char *testN(char *, int, int);
void testfns(void (*[])(int), int, int);

int main()
{
    void test1(int);
    void test2(int);
    void test3(int);
    void test4(int);
    void test5(int);
    void test6(int);

    int  ts[] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 2000, 4000};
    void (*fns[])(int) = { test1, test2, test3, test4, test5, test6 };
    char *Os[] = { "O(N)", "O(N^2)", "O(N^3)", "O(N^2)", "O(N^5)", "O(N^4)" };
    int  i, fnN = sizeof(fns) / sizeof(fns[0]);
    char s[10];

    printf("      |");
    for (i = 0; i < fnN; i++)
        printf(" %6s |", Os[i]);
    putchar('\n');
    for (i = 0; i < sizeof(ts) / sizeof(ts[0]); i++)
        testfns(fns, fnN, ts[i]);
}

char *testN(char *s, int tail, int n)
{
    char num[10];
    int i, nc = sprintf(num, "%d", n);
    for (i = 0; i <= nc; i++)
        s[tail++] = num[i];
    return s;
}

void testfns(void (*fns[])(int), int fnN, int N)
{
    int fntime(void (*)(int), int);
    int i;
    double t;

    printf("%5d |", N);
    for (i = 0; i < fnN; i++)
        printf(" %6d |", fntime(fns[i], N));
    putchar('\n');
}

int fntime(void (*fn)(int), int N)
{
    int i, start, end;
    start = clock();
    for (i = 0; i < TESTNUM; i++)
        fn(N);
    end = clock();
    return (end - start) / TESTNUM;
}

void test1(int N)
{
    int i, sum = 0;
    for (i = 0; i < N; i++)
        sum++;
}

void test2(int N)
{
    int i, j, sum = 0;
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            sum++;
}

void test3(int N)
{
    int i, j, sum = 0;
    for (i = 0; i < N; i++)
        for (j = 0; j < N * N; j++)
            sum++;
}

void test4(int N)
{
    int i, j, sum;
    for (i = 0; i < N; i++)
        for (j = 0; j < i; j++)
            sum++;
}

void test5(int N)
{
    int i, j, k, sum;
    for (i = 0; i < N; i++)
        for (j = 0; j < i * i; j++)
            for (k = 0; k < j; k++)
                sum++;
}

void test6(int N)
{
    int i, j, k, sum;
    for (i = 1; i < N; i++)
        for (j = 1; j < i * i; j++)
            if (j % i == 0)
                for (k = 0; k < j; k++)
                    sum++;
}