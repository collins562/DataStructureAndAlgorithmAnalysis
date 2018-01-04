#ifndef _MaxHeap_H

struct HeapStruct;
typedef struct HeapStruct *PriorityQueue;
typedef int ElementType;

PriorityQueue Init_Heap(int MaxElements);
void Destroy_Heap(PriorityQueue H);
static void PercolateUp(int Pos, PriorityQueue H);
static void PercolateDown(int Pos, PriorityQueue H);
int Find(ElementType X, PriorityQueue H);
void Insert(ElementType X, PriorityQueue H);
void Delete(int Pos, PriorityQueue H);
void DecreaseKey(int Pos, int Offset, PriorityQueue H);
void IncreaseKey(int Pos, int Offset, PriorityQueue H);
void BuildHeap(PriorityQueue H);
ElementType DeleteMax(PriorityQueue H);
ElementType FindMax(PriorityQueue H);
void PrintBinHeap(PriorityQueue H);
int IsEmpty(PriorityQueue H);
int IsFull(PriorityQueue H);

#endif