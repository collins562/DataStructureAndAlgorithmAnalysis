#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct DequeRecord;
struct Node;
typedef struct DequeRecord *Deque;
typedef struct Node *PtrToNode;
typedef PtrToNode Position;
typedef int ElementType;

int IsEmpty(Deque D);
int IsFull(Deque D);
Deque CreateDeque(void);
void DisposeDeque(Deque D);
void MakeEmpty(Deque D);
void Push(ElementType X, Deque D);
ElementType Pop(Deque D);
void Inject(ElementType X, Deque D);
ElementType Eject(Deque D);
ElementType Front(Deque D);
ElementType Rear(Deque D);
void Error(char *Message);

/* Deque implementation is a dynamically allocated array */

struct DequeRecord {
    int         size;
    Position    Front;
    Position    Rear;
};

struct Node {
    ElementType Element;
    Position    Previous;
    Position    Next;
};

int main()
{
    int i;
    Deque d;

    d = CreateDeque();
    for (i = 10; i > 0; i--)
        Push(i, d);
    for (i = 11; i < 20; i++)
        Inject(i, d);
    for (i = 0; i < 10; i++)
        Eject(d);
    printf("-----------\n");
    while (!IsEmpty(d)) {
        printf("%d ", Pop(d));
    }
}

int IsEmpty(Deque D)
{
    return D->size == 0;
}

Deque CreateDeque(void)
{
    Deque TmpCell;

    TmpCell = malloc(sizeof(struct DequeRecord));
    if (TmpCell == NULL)
        Error("CreateDeque: out of space");

    MakeEmpty(TmpCell);
    return TmpCell;
}

void DisposeDeque(Deque D)
{
    while (!IsEmpty(D))
        Pop(D);
    free(D);
}

void MakeEmpty(Deque D)
{
    D->size = 0;
    D->Front = NULL;
    D->Rear = NULL;
}

static Position NewNode(ElementType X, Position prev, Position next)
{
    Position TmpCell;

    if ((TmpCell = malloc(sizeof(struct Node))) == NULL)
        Error("NewNode: out of space");

    TmpCell->Element = X;
    TmpCell->Previous = prev;
    TmpCell->Next = next;

    return TmpCell;
}

void Push(ElementType X, Deque D)
{
    if (IsEmpty(D)) {
        D->Front = NewNode(X, NULL, NULL);
        D->Rear = D->Front;
    } else {
        D->Front->Previous = NewNode(X, NULL, D->Front);
        D->Front = D->Front->Previous;
    }
    D->size++;
}

ElementType Pop(Deque D)
{
    Position P;
    ElementType res;

    if (IsEmpty(D))
        Error("Pop: operate on empty deque");
    else {
        D->size--;
        P = D->Front;
        D->Front = D->Front->Next;
        if (D->Front != NULL)
            D->Front->Previous = NULL;
        res = P->Element;
        free(P);
        return res;
    }
}

void Inject(ElementType X, Deque D)
{
    if (IsEmpty(D)) {
        D->Front = NewNode(X, NULL, NULL);
        D->Rear = D->Front;
    } else {
        D->Rear->Next = NewNode(X, D->Rear, NULL);
        D->Rear = D->Rear->Next;
    }
    D->size++;
}

ElementType Eject(Deque D)
{
    Position P;
    ElementType res;

    if (IsEmpty(D))
        Error("Eject: operate on empty deque");
    else {
        D->size--;
        P = D->Rear;
        D->Rear = D->Rear->Previous;
        D->Rear->Next = NULL;
        res = P->Element;
        free(P);
        return res;
    }
}

ElementType Front(Deque D)
{
    if (IsEmpty(D))
        Error("Front: empty Deque");
    else
        return D->Front->Element;
}

ElementType Rear(Deque D)
{
    if (IsEmpty(D))
        Error("Rear: empty Deque");
    else
        return D->Rear->Element;
}

void Error(char *Message)
{
    fprintf(stderr, "%s\n", Message);
    // exit(1);
}