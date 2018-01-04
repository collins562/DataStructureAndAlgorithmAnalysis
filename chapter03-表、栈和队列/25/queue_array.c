#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MinQueueSize 5

struct QueueRecord;
typedef struct QueueRecord *Queue;
typedef int ElementType;

int IsEmpty(Queue Q);
int IsFull(Queue Q);
Queue CreateQueue(int MaxElements);
void DisposeQueue(Queue Q);
void MakeEmpty(Queue Q);
void Enqueue(ElementType X, Queue Q);
ElementType Front(Queue Q);
void Dequeue(Queue Q);
ElementType FrontAndDequeue(Queue Q);
void Error(char *Message);

/* Queue implementation is a dynamically allocated array */

struct QueueRecord {
    int Capacity;
    int Front;
    int Rear;
    int size;
    ElementType *Array;
};

int IsEmpty(Queue Q)
{
    return Q->size == 0;
}

int IsFull(Queue Q)
{
    return Q->size == Q->Capacity - 1;
}

Queue CreateQueue(int MaxElements)
{
    Queue TmpCell;

    if (MaxElements < MinQueueSize)
        MaxElements = MinQueueSize;

    TmpCell = malloc(sizeof(struct QueueRecord));
    if (TmpCell == NULL)
        Error("CreateQueue: out of space");

    MakeEmpty(TmpCell);
    TmpCell->Capacity = MaxElements;
    TmpCell->Array = malloc(sizeof(ElementType) * MaxElements);

    return TmpCell;
}

void DisposeQueue(Queue Q)
{
    free(Q->Array);
    free(Q);
}

void MakeEmpty(Queue Q)
{
    Q->size = 0;
    Q->Front = 1;
    Q->Rear = 0;
}

static int Succ(int Value, Queue Q)
{
    if (++Value == Q->Capacity)
        Value = 0;
    return Value;
}

void Enqueue(ElementType X, Queue Q)
{
    if (IsFull(Q))
        Error("Full queue");
    else {
        Q->size++;
        Q->Rear = Succ(Q->Rear, Q);
        Q->Array[Q->Rear] = X;
    }
}


ElementType Front(Queue Q)
{
    return Q->Array[Front];
}

void Dequeue(Queue Q)
{
    if (IsEmpty(Q))
        Error("Empty queue");
    else {
        Q->size--;
        Q->Front = Succ(Q->Front, Q);
    }
}

ElementType FrontAndDequeue(Queue Q)
{
    ElementType res;

    res = Front(Q);
    Dequeue(Q);
    return res;
}

void Error(char *Message)
{
    fprintf(stderr, "%s\n", Message);
    // exit(1);
}