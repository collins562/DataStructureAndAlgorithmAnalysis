#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "MinMaxHeap.h"

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

int gt(ElementType x, ElementType y)
{
    return x > y;
}

int lt(ElementType x, ElementType y)
{
    return x < y;
}

static int BitSize(int x)
{
    int res;

    for (res = -1; x > 0; res++)
        x >>= 1;
    return res;
}

/* percolate up on even level                                   */
void ELPercolateUp(int Pos, PriorityQueue H)
{
    int i;
    ElementType X;

    i = Pos, X = H->Elements[Pos];
    // Min-Max Heap's requires of X: 
    //     H->Elements[i/4] < X < H->Elements[i/2] (i >= 4)
    if (X > H->Elements[i/2] && i != 1) {
        H->Elements[i] = H->Elements[i/2];
        for (i /= 2; i / 4 != 0 && H->Elements[i/4] < X; i /= 4)
            H->Elements[i] = H->Elements[i/4];
    } else
        while (H->Elements[i/4] > X) {
            H->Elements[i] = H->Elements[i/4];
            i /= 4;
        }
    H->Elements[i] = X;
}

/* percolate up on odd level                                    */
void OLPercolateUp(int Pos, PriorityQueue H)
{
    int i;
    ElementType X;

    i = Pos, X = H->Elements[Pos];
    // Min-Max Heap's requires of X:
    //     H->Elements[i/2] < X < H->Elements[i/4] (i >= 8)
    if (H->Elements[i/2] > X) {
        H->Elements[i] = H->Elements[i/2];
        for (i /= 2; H->Elements[i/4] > X; i /= 4)
            H->Elements[i] = H->Elements[i/4];
    } else
        while (i / 4 != 0 && H->Elements[i/4] < X) {
            H->Elements[i] = H->Elements[i/4];
            i /= 4;
        }
    H->Elements[i] = X;
}

void PercolateUp(int Pos, PriorityQueue H)
{
    if (BitSize(Pos) % 2 == 0)
        ELPercolateUp(Pos, H);
    else
        OLPercolateUp(Pos, H);
}

void PercolateDown(int Pos, PriorityQueue H)
{
    int lt(ElementType, ElementType);
    int gt(ElementType, ElementType);
    int (*Compare)(ElementType, ElementType);

    int i, j, Child, First;
    ElementType X;

    Compare = (BitSize(Pos) % 2 == 0) ? lt : gt;

    X = H->Elements[Pos];
    // Min-Max Heap's requires of X:
    //     (Even Level) X < H->Elements[i*4 ~ i*4+3] < H->Elements[i*2, i*2+1]
    //     (Odd  Level) X > H->Elements[i*4 ~ i*4+3] > H->Elements[i*2, i*2+1]
    for (i = Pos; i * 4 <= H->Size; i = Child) {
        // find smaller / bigger Child
        First = Child = i * 4;
        for (j = 1; First + j <= H->Size && j < 4; j++)
            if (Compare(H->Elements[First + j], H->Elements[Child]))
                Child = First + j;

        if (j <= 2 && Compare(H->Elements[i*2+1], H->Elements[Child]))
            Child = i * 2 + 1;
        
        if (Compare(H->Elements[Child], X))
            H->Elements[i] = H->Elements[Child];
        else
            break;
    }

    if (i * 4 > H->Size && i * 2 <= H->Size) {
        // find smaller Child in next level
        Child = i * 2;
        if (Child != H->Size && Compare(H->Elements[Child+1], H->Elements[Child]))
            Child++;

        if (Compare(H->Elements[Child], X)) {
            H->Elements[i] = H->Elements[Child];
            i = Child;
        }
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

void BuildHeap(PriorityQueue H)
{
    int i;

    for (i = H->Size / 2; i > 0; i--)
        PercolateDown(i, H);
}

ElementType DeleteMin(PriorityQueue H)
{
    ElementType MinElement;

    if (IsEmpty(H)) {
        Error("Priority queue is Empty");
        return H->Elements[0];
    }
    MinElement = H->Elements[1];
    H->Elements[1] = H->Elements[H->Size--];    // move last element
    PercolateDown(1, H);

    return MinElement;
}

ElementType DeleteMax(PriorityQueue H)
{
    int i;
    ElementType MaxElement;

    if (IsEmpty(H)) {
        Error("Priority queue is Empty");
        return H->Elements[0];
    } else if (H->Size <= 2)
        return H->Elements[H->Size--];
    
    i = (H->Elements[2] > H->Elements[3]) ? 2 : 3;
    MaxElement = H->Elements[i];
    H->Elements[i] = H->Elements[H->Size--];    // move last element
    PercolateDown(i, H);

    return MaxElement;
}

ElementType FindMin(PriorityQueue H)
{
    return (IsEmpty(H)) ? H->Elements[0] : H->Elements[1];
}

ElementType FindMax(PriorityQueue H)
{
    if (H->Size < 2)
        return H->Elements[H->Size];
    else if (H->Size == 2)
        return H->Elements[2];
    else
        return (H->Elements[1] > H->Elements[2]) ? H->Elements[1] : H->Elements[2];
}

int IsEmpty(PriorityQueue H)
{
    return H->Size == 0;
}

int IsFull(PriorityQueue H)
{
    return H->Size >= H->Capacity;
}

void PrintMinMaxHeap(PriorityQueue H)
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
