#ifndef _BinHeap_H

struct HeapStruct;
typedef struct HeapStruct *PriorityQueue;
typedef Edge HeapElementType;

PriorityQueue InitBinHeap(int MaxElements);
void Destroy(PriorityQueue H);
void MakeEmptyHeap(PriorityQueue H);
void RawInsert(HeapElementType X, PriorityQueue H);
void Insert(HeapElementType X, PriorityQueue H);
void DecreaseKey(int Pos, int Offset, PriorityQueue H);
void IncreaseKey(int Pos, int Offset, PriorityQueue H);
void Delete (int Pos, PriorityQueue H);
void BuildHeap(PriorityQueue H);
HeapElementType DeleteMin(PriorityQueue H);
HeapElementType FindMin(PriorityQueue H);
int IsEmptyHeap(PriorityQueue H);
int IsFullHeap(PriorityQueue H);
void PrintBinHeap(PriorityQueue H);

struct HeapStruct {
    int Capacity;
    int Size;
    HeapElementType *Elements;
};

#endif