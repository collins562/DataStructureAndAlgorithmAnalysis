#include <stdio.h>
#include <stdlib.h>
#include "Sort.h"

#define Cutoff          3
#define LeftChild(i)    (2 * (i) + 1)
#define Min(X, Y)       ((X) < (Y)) ? (X) : (Y)
#define Error(Message)  fprintf(stderr, "%s\n", Message)

/* Insertion Sort */
void InsertionSort(ElementType A[], int N)
{
    int j, P;

    ElementType Tmp;
    for (P = 1; P < N; P++) {
        Tmp = A[P];
        for (j = P; j > 0 && A[j-1] > Tmp; j--)
            A[j] = A[j - 1];
        A[j] = Tmp;
    }
}

/* Shell Sort */
static int SedgewickIncSeq[] = { 28,
        1       , 5       , 19      , 41       , 109      , 209      , 505       , 
        929     , 2161    , 3905    , 8929     , 16001    , 36289    , 64769     , 
        146305  , 260609  , 587521  , 1045505  , 2354689  , 4188161  , 9427969   , 
        16764929, 37730305, 67084289, 150958081, 268386305, 603906049, 1073643521 };

static int SedgewickIncIndex(int N)
{
    int low, high, mid;

    if (N < SedgewickIncSeq[2])
        return 1;
    else if (N > SedgewickIncSeq[SedgewickIncSeq[0]])
        return SedgewickIncSeq[0];

    low = 2; high = SedgewickIncSeq[0];
    while (low <= high) {
        mid = (low + high) / 2;
        if (N > SedgewickIncSeq[mid]) {
            if (N < SedgewickIncSeq[mid+1])
                return mid;
            else
                low = mid;
        } else if (N < SedgewickIncSeq[mid]) {
            if (N > SedgewickIncSeq[mid-1])
                return mid-1;
            else
                high = mid;
        } else
            return mid-1;
    }
}

void ShellSort(ElementType A[], int N)
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

/* Heap Sort */
void PercDown(ElementType A[], int i, int N)
{
    int Child;
    ElementType Tmp;

    for (Tmp = A[i]; LeftChild(i) < N; i = Child) {
        Child = LeftChild(i);
        if (Child != N - 1 && A[Child + 1] > A[Child])
            Child++;
        if (Tmp < A[Child])
            A[i] = A[Child];
        else
            break;
    }
    A[i] = Tmp;
}

void HeapSort(ElementType A[], int N)
{
    void Swap(ElementType *, ElementType *);
    int i;

    for (i = N / 2; i >= 0; i--)
        PercDown(A, i, N);
    for (i = N - 1; i > 0; i--) {
        Swap(&A[0], &A[i]);
        PercDown(A, 0, i);
    }
}

void Swap(ElementType *x, ElementType *y)
{
    ElementType Tmp = *x;
    *x = *y;
    *y = Tmp;
}

/* Merge Sort */
void Merge(ElementType A[], ElementType TmpArray[], int Lpos, int Rpos, int RightEnd)
{
    int i, LeftEnd, NumElements, TmpPos;

    LeftEnd = Rpos - 1;
    TmpPos = Lpos;
    NumElements = RightEnd - Lpos + 1;

    /* main loop */
    while (Lpos <= LeftEnd && Rpos <= RightEnd)
        if (A[Lpos] <= A[Rpos])
            TmpArray[TmpPos++] = A[Lpos++];
        else
            TmpArray[TmpPos++] = A[Rpos++];

    while (Lpos <= LeftEnd)
        TmpArray[TmpPos++] = A[Lpos++];
    while (Rpos <= RightEnd)
        TmpArray[TmpPos++] = A[Rpos++];

    /* Copy TmpArray back */
    for (i = 0; i < NumElements; i++, RightEnd--)
        A[RightEnd] = TmpArray[RightEnd];
}

void MergeSort(ElementType A[], int N)
{
    ElementType *TmpArray;
    int SubListSize, Part1Start, Part2Start, Part2End;

    TmpArray = malloc(N * sizeof(ElementType));
    if (TmpArray == NULL) {
        Error("No space for tmp array!");
        return;
    }

    for (SubListSize = 1; SubListSize < N; SubListSize *= 2) {
        Part1Start = 0;
        while (Part1Start + SubListSize < N - 1) {
            Part2Start = Part1Start + SubListSize;
            Part2End = Min(N, Part2Start + SubListSize - 1);
            Merge(A, TmpArray, Part1Start, Part2Start, Part2End);
            Part1Start = Part2End + 1;
        }
    }
    free(TmpArray);
}

/* Quick Sort */
ElementType Median3(ElementType A[], int Left, int Right)
{
    void Swap(ElementType *, ElementType *);
    int Center = (Left + Right) / 2;

    if (A[Left] > A[Center])
        Swap(&A[Left], &A[Center]);
    if (A[Left] > A[Right])
        Swap(&A[Left], &A[Right]);
    if (A[Center] > A[Right])
        Swap(&A[Center], &A[Right]);

    /* Invariant: A[Left] <= A[Center] <= A[Right]   */
    Swap(&A[Center], &A[Right - 1]); /* Hide pivot   */
    return A[Right - 1];             /* Return pivot */
}

void QSort(ElementType A[], int Left, int Right)
{
    int i, j;
    ElementType Pivot;

    if (Left + Cutoff <= Right) {
        Pivot = Median3(A, Left, Right);
        i = Left; j = Right - 1;
        for ( ; ; ) {
            while (A[++i] < Pivot) {}
            while (A[--j] > Pivot) {}
            if (i < j)
                Swap(&A[i], &A[j]);
            else
                break;
        }
        Swap(&A[i], &A[Right - 1]); /* Restore pivot */

        QSort(A, Left, i - 1);
        QSort(A, i + 1, Right);
    } else /* Do an insertion sort on the subarray   */
        InsertionSort(A + Left, Right - Left + 1);
}

void QuickSort(ElementType A[], int N)
{
    QSort(A, 0, N - 1);
}

void PrintArray(ElementType A[], int N)
{
    int i;

    for (i = 0; i < N; i++)
        printf("%d ", A[i]);
    putchar('\n');
}