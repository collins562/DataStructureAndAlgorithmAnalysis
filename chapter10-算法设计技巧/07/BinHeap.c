#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Huffman.h"
#include "BinHeap.h"

// #define MinData     -1
#define MinPQSize  7
#define HeapEltcmp huftreecmp
#define Error(Message) fprintf(stderr, "%s\n", Message)

PriorityQueue InitBinHeap(int MaxElements)
{
    PriorityQueue H;

    if (MaxElements < MinPQSize)
        Error("Priority queue size is too small");

    H = malloc(sizeof(struct HeapStruct));
    if (H == NULL)
        Error("Out of space");

    /* Allocate the array plus onr extra for sentinel */
    H->Elements = malloc((MaxElements + 1) * sizeof(HeapElementType));

    if (H->Elements == NULL)
        Error("Out of space");

    H->Capacity = MaxElements;
    H->Size = 0;
    H->Elements[0] = create_node(0, -1, -1, -1);

    return H;
}

void RawInsert(HeapElementType X, PriorityQueue H)
{
    if (IsFullHeap(H)) {
        Error("Priority queue is full");
        return;
    }

    H->Elements[++H->Size] = X;
}

void Destroy(PriorityQueue H)
{
    free(H->Elements);
    free(H);
}

void MakeEmptyHeap(PriorityQueue H)
{
    H->Capacity = MinPQSize;
    H->Size = 0;
}

static void PercolateUp(int Pos, PriorityQueue H)
{
    int i;
    HeapElementType X;

    X = H->Elements[Pos];
    for (i = Pos; HeapEltcmp(H->Elements[i/2], X) > 0; i /= 2)
        H->Elements[i] = H->Elements[i/2];
    H->Elements[i] = X;
}

static void PercolateDown(int Pos, PriorityQueue H)
{
    int i, Child;
    HeapElementType X;

    X = H->Elements[Pos];
    for (i = Pos; i * 2 <= H->Size; i = Child) {
        // Find smaller Child
        Child = i * 2;
        if (Child != H->Size && HeapEltcmp(H->Elements[Child + 1], 
                                           H->Elements[Child]) < 0)
            Child++;

        // Percolate one level
        if (HeapEltcmp(X, H->Elements[Child]) > 0)
            H->Elements[i] = H->Elements[Child];
        else
            break;
    }
    H->Elements[i] = X;
}

void Insert(HeapElementType X, PriorityQueue H)
{
    RawInsert(X, H);
    PercolateUp(H->Size, H);
}

void BuildHeap(PriorityQueue H)
{
    int i;

    for (i = H->Size / 2; i > 0; i--)
        PercolateDown(i, H);
}

HeapElementType DeleteMin(PriorityQueue H)
{
    HeapElementType MinElement, LastElement;

    if (IsEmptyHeap(H)) {
        Error("Priority queue is Empty");
        return H->Elements[0];
    }
    MinElement = H->Elements[1];
    LastElement = H->Elements[H->Size--];
    H->Elements[1] = LastElement;
    PercolateDown(1, H);

    return MinElement;
}

int IsEmptyHeap(PriorityQueue H)
{
    return H->Size == 0;
}

int IsFullHeap(PriorityQueue H)
{
    return H->Size >= H->Capacity;
}