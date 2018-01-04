#include <stdio.h>
#include <stdlib.h>
#include "maxheap.h"

#define Infinity 10000  // Infinity should be changed when elt_type changes
#define Error(Message) fprintf(stderr, "%s\n", Message);

struct HeapStruct {
    int Capacity;
    int Size;
    ElementType *Elements;
};

PriorityQueue Init_Heap(int MaxElements)
{
    PriorityQueue H;

    H = malloc(sizeof(struct HeapStruct));
    if (H != NULL) {
        H->Elements = malloc((MaxElements + 1) * sizeof(ElementType));
        if (H->Elements == NULL)
            return NULL;
        H->Capacity = MaxElements;
        H->Size = 0;
        H->Elements[0] = Infinity;
    }

    return H;
}

void Destroy_Heap(PriorityQueue H)
{
    free(H->Elements);
    free(H);
}

static void PercolateUp(int Pos, PriorityQueue H)
{
    int i;
    ElementType X;

    X = H->Elements[Pos];
    for (i = Pos; H->Elements[i/2] < X; i /= 2)
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
        if (Child != H->Size && H->Elements[Child + 1] > H->Elements[Child])
            Child++;

        // Percolate one level
        if (X < H->Elements[Child])
            H->Elements[i] = H->Elements[Child];
        else
            break;
    }
    H->Elements[i] = X;
}

int Find(ElementType X, PriorityQueue H)
{
    int i, pos;

    pos = -1;
    for (i = 1; i <= H->size; i++)
        if (H->Elements[i] == X) {
            pos = i;
            break;
        }

    return pos;
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

void Delete(int Pos, PriorityQueue H)
{
    ElementType LastElement;

    if (Pos < 1) {
        Error("Delete: Illegal position");
        return;
    } else if (Pos > H->Size) {
        Error("Delete: Out of record");
        return;
    }

    LastElement = H->Elements[H->Size--];
    H->Elements[Pos] = LastElement;
    PercolateDown(Pos, H);
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
    
    PercolateDown(Pos, H);
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
    PercolateUp(Pos, H);
}

void BuildHeap(PriorityQueue H)
{
    int i;

    for (i = H->Size / 2; i > 0; i--)
        PercolateDown(i, H);
}

ElementType DeleteMax(PriorityQueue H)
{
    ElementType MaxElement, LastElement;

    if (IsEmpty(H)) {
        Error("Priority queue is Empty");
        return H->Elements[0];
    }
    MaxElement = H->Elements[1];
    Delete(1, H);

    return MaxElement;
}

ElementType FindMax(PriorityQueue H)
{
    return (IsEmpty(H)) ? H->Elements[0] : H->Elements[1];
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

int IsEmpty(PriorityQueue H)
{
    return H->Size == 0;
}

int IsFull(PriorityQueue H)
{
    return H->Size >= H->Capacity;
}