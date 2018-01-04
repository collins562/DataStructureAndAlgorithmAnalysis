#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "BinHeap.h"

#define MinData     -1
#define MinPQSize   7
#define Error(Message) fprintf(stderr, "%s\n", Message)


PriorityQueue Initialize(int MaxElements)
{
    PriorityQueue H;

    if (MaxElements < MinPQSize)
        Error("Priority queue size is too small");

    H = malloc(sizeof(struct HeapStruct));
    if (H == NULL)
        Error("Out of space");

    /* Allocate the array plus onr extra for sentinel */
    H->Elements = malloc((MaxElements + 1) * sizeof(ElementType));

    if (H->Elements == NULL)
        Error("Out of space");

    H->Capacity = MaxElements;
    H->Size = 0;
    H->Elements[0] = MinData;

    return H;
}

void Destroy(PriorityQueue H)
{
    free(H->Elements);
    free(H);
}

void MakeEmpty(PriorityQueue H)
{
    H->Capacity = MinPQSize;
    H->Size = 0;
}

static void PercolateUp(int Pos, PriorityQueue H)
{
    int i;
    ElementType X;

    X = H->Elements[Pos];
    for (i = Pos; H->Elements[i/2] > X; i /= 2)
        H->Elements[i] = H->Elements[i/2];
    H->Elements[i] = X;
}

static void PercolateDown(int Pos, PriorityQueue H)
{
    int i, Child;
    ElementType X;

    X = H->Elements[Pos];
    for (i = Pos; i * 2 <= H->Size; i = Child) {
        // Find smaller Child
        Child = i * 2;
        if (Child != H->Size && H->Elements[Child + 1] < H->Elements[Child])
            Child++;

        // Percolate one level
        if (X > H->Elements[Child])
            H->Elements[i] = H->Elements[Child];
        else
            break;
    }
    H->Elements[i] = X;
}

void Insert(ElementType X, PriorityQueue H)
{
    int i;

    if (IsFull(H)) {
        Error("Priority queue is full");
        return;
    }

    H->Elements[++H->Size] = X;
    PercolateUp(H->Size, H);
}

/* Decrease the Key in Pos by Offset (positive integer) */
void DecreaseKey(int Pos, int Offset, PriorityQueue H)
{
    if (Pos < 1) {
        Error("DecreaseKey: Illegal position");
        return;
    } else if (Pos > H->Size) {
        Error("DecreaseKey: Out of record");
        return;
    }

    H->Elements[Pos] -= Offset;
    
    if (H->Elements[Pos] < 1)
        Error("Less than highest priority");   // Assume Key 1 has highest priority
    
    PercolateUp(Pos, H);
}

/* Increase the Key in Pos by Offset (positive integer) */
void IncreaseKey(int Pos, int Offset, PriorityQueue H)
{
    if (Pos < 1) {
        Error("IncreaseKey: Illegal position");
        return;
    } else if (Pos > H->Size) {
        Error("IncreaseKey: Out of record");
        return;
    }

    H->Elements[Pos] += Offset;
    PercolateDown(Pos, H);
}

void Delete (int Pos, PriorityQueue H)
{
    if (Pos < 1) {
        Error("Delete: Illegal position");
        return;
    } else if (Pos > H->Size) {
        Error("Delete: Out of record");
        return;
    }

    DecreaseKey(Pos, H->Elements[Pos], H);
    DeleteMin(H);
}

void BuildHeap(PriorityQueue H)
{
    int i;

    for (i = H->Size / 2; i > 0; i--)
        PercolateDown(i, H);
}

ElementType DeleteMin(PriorityQueue H)
{
    ElementType MinElement, LastElement;

    if (IsEmpty(H)) {
        Error("Priority queue is Empty");
        return H->Elements[0];
    }
    MinElement = H->Elements[1];
    LastElement = H->Elements[H->Size--];
    H->Elements[1] = LastElement;
    PercolateDown(1, H);

    return MinElement;
}

ElementType FindMin(PriorityQueue H)
{
    return (IsEmpty(H)) ? H->Elements[0] : H->Elements[1];
}

int IsEmpty(PriorityQueue H)
{
    return H->Size == 0;
}

int IsFull(PriorityQueue H)
{
    return H->Size >= H->Capacity;
}

void PrintBinHeap(PriorityQueue H)
{
    int i, j, Level, LevelSize;

    for (i = 1, Level = 1, LevelSize = 1; i <= H->Size; Level++) {
        printf("Level %02d: ", Level);
        for (j = LevelSize; j > 0 && i <= H->Size; i++, j--)
            printf("%d ", H->Elements[i]);
        putchar('\n');
        LevelSize <<= 1;
    }
}

void PrintUnder(ElementType X, PriorityQueue H)
{
    void PrintUnder1(ElementType, int, PriorityQueue);

    printf("Under %d: ", X);
    PrintUnder1(X, 1, H);
}

void PrintUnder1(ElementType X, int Pos, PriorityQueue H)
{
    if (Pos <= H->Size && H->Elements[Pos] < X) {
        printf("%d ", H->Elements[Pos]);
        PrintUnder1(X, Pos * 2, H);
        PrintUnder1(X, Pos * 2 + 1, H);
    }
}