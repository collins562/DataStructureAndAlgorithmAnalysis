#ifndef _MinMaxHeap_H

struct HeapStruct;
typedef struct HeapStruct *PriorityQueue;
typedef int ElementType;

PriorityQueue Initialize(int MaxElements);
void Destroy(PriorityQueue H);
void MakeEmpty(PriorityQueue H);
void Insert(ElementType X, PriorityQueue H);
void BuildHeap(PriorityQueue H);
ElementType DeleteMin(PriorityQueue H);
ElementType DeleteMax(PriorityQueue H);
ElementType FindMin(PriorityQueue H);
ElementType FindMax(PriorityQueue H);
int IsEmpty(PriorityQueue H);
int IsFull(PriorityQueue H);
void PrintMinMaxHeap(PriorityQueue H);

struct HeapStruct {
    int Capacity;
    int Size;
    ElementType *Elements;
};

#endif