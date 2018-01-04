#ifndef _BinHeap_H

struct HeapStruct;
typedef struct HeapStruct *PriorityQueue;
typedef int ElementType;

PriorityQueue Initialize(int MaxElements);
void Destroy(PriorityQueue H);
void MakeEmpty(PriorityQueue H);
void Insert(ElementType X, PriorityQueue H);
void DecreaseKey(int Pos, int Offset, PriorityQueue H);
void IncreaseKey(int Pos, int Offset, PriorityQueue H);
void Delete (int Pos, PriorityQueue H);
void BuildHeap(PriorityQueue H);
ElementType DeleteMin(PriorityQueue H);
ElementType FindMin(PriorityQueue H);
int IsEmpty(PriorityQueue H);
int IsFull(PriorityQueue H);
void PrintBinHeap(PriorityQueue H);
void PrintUnder(ElementType X, PriorityQueue H);

struct HeapStruct {
    int Capacity;
    int Size;
    ElementType *Elements;
};

#endif