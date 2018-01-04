#ifndef _BinHeap_H

struct HeapStruct;
typedef struct HeapStruct *PriorityQueue;
typedef htnp HeapElementType;

PriorityQueue InitBinHeap(int MaxElements);
void Destroy(PriorityQueue H);
void MakeEmptyHeap(PriorityQueue H);
void RawInsert(HeapElementType X, PriorityQueue H);
void Insert(HeapElementType X, PriorityQueue H);
void BuildHeap(PriorityQueue H);
HeapElementType DeleteMin(PriorityQueue H);
int IsEmptyHeap(PriorityQueue H);
int IsFullHeap(PriorityQueue H);

struct HeapStruct {
    int Capacity;
    int Size;
    HeapElementType *Elements;
};

#endif