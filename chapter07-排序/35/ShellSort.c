#include <stdio.h>
#include <stdlib.h>
#include "ShellSort.h"

#define Error(Message)  fprintf(stderr, "%s\n", Message)

static int IncIndex(int IncSeq[], int N)
{
    int low, high, mid;

    if (N < IncSeq[2])
        return 1;
    else if (N > IncSeq[IncSeq[0]])
        return IncSeq[0];

    low = 2; high = IncSeq[0];
    while (low <= high) {
        mid = (low + high) / 2;
        if (N > IncSeq[mid]) {
            if (N < IncSeq[mid+1])
                return mid;
            else
                low = mid;
        } else if (N < IncSeq[mid]) {
            if (N > IncSeq[mid-1])
                return mid-1;
            else
                high = mid;
        } else
            return mid-1;
    }
}

/* Shell Sort - original */
void ShellSort1(ElementType A[], int N)
{
    int i, j, Inc;
    ElementType Tmp;

    for (Inc = N / 2; Inc > 0; Inc /= 2)
        for (i = Inc; i < N; i++) {
            Tmp = A[i];
            for (j = i; j >= Inc; j -= Inc)
                if (Tmp < A[j - Inc])
                    A[j] = A[j - Inc];
                else
                    break;
            A[j] = Tmp;
        }
}

/* Shell Sort - Hibbard increment */
static int BitSize(int N)
{
    int Size;

    for (Size = 0; N > 0; Size++)
        N >>= 1;
    return Size;
}

void ShellSort2(ElementType A[], int N)
{
    int i, j, Inc;
    ElementType Tmp;

    for (Inc = 1 << BitSize(N) - 1; Inc > 0; Inc = (Inc - 1) / 2)
        for (i = Inc; i < N; i++) {
            Tmp = A[i];
            for (j = i; j >= Inc; j -= Inc)
                if (Tmp < A[j - Inc])
                    A[j] = A[j - Inc];
                else
                    break;
            A[j] = Tmp;
        }
}

/* Shell Sort - Knuth increment */
static int KnuthIncSeq[] = { 
        20 ,
        1       , 2       , 5        , 14       , 41    , 122   , 365    , 1094   , 
        3281    , 9842    , 29525    , 88574    , 265721, 797162, 2391485, 7174454, 
        21523361, 64570082, 193710245, 581130734
};

static int KnuthIncIndex(int N)
{
    return IncIndex(KnuthIncSeq, N);
}

void ShellSort3(ElementType A[], int N)
{
    int i, j, Inc, HIndex;
    ElementType Tmp;

    for (HIndex = KnuthIncIndex(N); HIndex > 0; HIndex--)
        for (i = Inc = KnuthIncSeq[HIndex]; i < N; i++) {
            Tmp = A[i];
            for (j = i; j >= Inc; j -= Inc)
                if (Tmp < A[j - Inc])
                    A[j] = A[j - Inc];
                else
                    break;
            A[j] = Tmp;
        }
}

/* Shell Sort - Gonnet increment */
void ShellSort4(ElementType A[], int N)
{
    int i, j, Inc;
    ElementType Tmp;

    for (Inc = N / 2.2; Inc > 0; Inc = (Inc == 2) ? 1 : Inc/2.2)
        for (i = Inc; i < N; i++) {
            Tmp = A[i];
            for (j = i; j >= Inc; j -= Inc)
                if (Tmp < A[j - Inc])
                    A[j] = A[j - Inc];
                else
                    break;
            A[j] = Tmp;
        }
}

/* Shell Sort - Sedgewick increment */
static int SedgewickIncSeq[] = { 
        28      ,
        1       , 5       , 19      , 41       , 109      , 209      , 505       , 
        929     , 2161    , 3905    , 8929     , 16001    , 36289    , 64769     , 
        146305  , 260609  , 587521  , 1045505  , 2354689  , 4188161  , 9427969   , 
        16764929, 37730305, 67084289, 150958081, 268386305, 603906049, 1073643521 
};

int SedgewickIncIndex(int N)
{
    return IncIndex(SedgewickIncSeq, N);
}

void ShellSort5(ElementType A[], int N)
{
    int i, j, Inc, SIndex;
    ElementType Tmp;

    for (SIndex = SedgewickIncIndex(N); SIndex > 0; SIndex--)
        for (i = Inc = SedgewickIncSeq[SIndex]; i < N; i++) {
            Tmp = A[i];
            for (j = i; j >= Inc; j -= Inc)
                if (Tmp < A[j - Inc])
                    A[j] = A[j - Inc];
                else
                    break;
            A[j] = Tmp;
        }
}



void PrintArray(ElementType A[], int N)
{
    int i;

    for (i = 0; i < N; i++)
        printf("%d ", A[i]);
    putchar('\n');
}