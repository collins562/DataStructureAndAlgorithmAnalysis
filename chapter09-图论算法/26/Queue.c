#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Queue.h"

#define Error(Message) fprintf(stderr, "%s\n", Message)

/* Queue implementation is a dynamically allocated array */

struct QueueRecord {
    int         size;
    Position    Front;
    Position    Rear;
};

struct Node {
    ElementType Element;
    Position    Next;
};

int IsEmptyQueue(Queue Q)
{
    return Q->size == 0;
}

Queue CreateQueue(void)
{
    Queue TmpCell;

    TmpCell = malloc(sizeof(struct QueueRecord));
    if (TmpCell == NULL)
        Error("CreateQueue: out of space");

    MakeEmptyQueue(TmpCell);
    return TmpCell;
}

void DisposeQueue(Queue Q)
{
    Position P, Tmp;

    for (P = Q->Front; P != NULL; P = Tmp) {
        Tmp = P->Next;
        free(P);
    }
    free(Q->Front);
    free(Q->Rear);
    free(Q);
}

void MakeEmptyQueue(Queue Q)
{
    Q->size = 0;
    Q->Front = NULL;
    Q->Rear = Q->Front;
}

void Enqueue(ElementType X, Queue Q)
{
    Position TmpCell;

    if ((TmpCell = malloc(sizeof(struct Node))) == NULL)
        Error("Enqueue: out of space");

    TmpCell->Element = X;
    TmpCell->Next = NULL;
    if (IsEmptyQueue(Q))
        Q->Front = TmpCell;
    else
        Q->Rear->Next = TmpCell;
    Q->Rear = TmpCell;
    Q->size++;
}

ElementType Front(Queue Q)
{
    if (IsEmptyQueue(Q))
        Error("Front: empty queue");
    else
        return Q->Front->Element;
}

ElementType Dequeue(Queue Q)
{
    ElementType First;
    Position P;

    if (IsEmptyQueue(Q))
        Error("Empty queue");
    else {
        Q->size--;
        P = Q->Front;
        First = P->Element;
        Q->Front = Q->Front->Next;
        free(P);
    }
    return First;
}

ElementType FrontAndDequeue(Queue Q)
{
    ElementType res;

    res = Front(Q);
    Dequeue(Q);
    return res;
}

int QueueIndex(ElementType Elt, Queue Q)
{
    int index = 0;
    Position P;

    for (P = Q->Front; P != NULL; P = P->Next, index++)
        if (P->Element == Elt)
            return index;
    return -1;
}