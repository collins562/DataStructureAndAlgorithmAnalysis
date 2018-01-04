#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct QueueRecord;
struct Node;
typedef struct QueueRecord *Queue;
typedef struct Node *PtrToNode;
typedef PtrToNode Position;
typedef int ElementType;

int IsEmpty(Queue Q);
int IsFull(Queue Q);
Queue CreateQueue(void);
void DisposeQueue(Queue Q);
void MakeEmpty(Queue Q);
void Enqueue(ElementType X, Queue Q);
ElementType Front(Queue Q);
void Dequeue(Queue Q);
ElementType FrontAndDequeue(Queue Q);
void Error(char *Message);

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

int main()
{
    int i;
    Queue q;

    q = CreateQueue();
    for (i = 0; i < 10; i++)
        Enqueue(i, q);
    while (!IsEmpty(q)) {
        printf("%d ", FrontAndDequeue(q));
    }
}

int IsEmpty(Queue Q)
{
    return Q->size == 0;
}

Queue CreateQueue(void)
{
    Queue TmpCell;

    TmpCell = malloc(sizeof(struct QueueRecord));
    if (TmpCell == NULL)
        Error("CreateQueue: out of space");

    MakeEmpty(TmpCell);
    return TmpCell;
}

void DisposeQueue(Queue Q)
{
    free(Q->Front);
    free(Q->Rear);
    free(Q);
}

void MakeEmpty(Queue Q)
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
    if (IsEmpty(Q))
        Q->Front = TmpCell;
    else
        Q->Rear->Next = TmpCell;
    Q->Rear = TmpCell;
    Q->size++;
}

ElementType Front(Queue Q)
{
    if (IsEmpty(Q))
        Error("Front: empty queue");
    else
        return Q->Front->Element;
}

void Dequeue(Queue Q)
{
    Position P;

    if (IsEmpty(Q))
        Error("Empty queue");
    else {
        Q->size--;
        P = Q->Front;
        Q->Front = Q->Front->Next;
        free(P);
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