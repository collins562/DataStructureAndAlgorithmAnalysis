#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Deque.h"

#define Error(Message) fprintf(stderr, "%s\n", Message)

/* Deque implementation is a dynamically allocated array */

struct DequeRecord {
    int         Size;
    Position    Front;
    Position    Rear;
};

struct Node {
    ElementType Element;
    Position    Previous;
    Position    Next;
};

int IsEmptyDeque(Deque D)
{
    return D->Size == 0;
}

Deque CreateDeque(void)
{
    Deque TmpCell;

    TmpCell = malloc(sizeof(struct DequeRecord));
    if (TmpCell == NULL)
        Error("CreateDeque: out of space");

    MakeEmptyDeque(TmpCell);
    return TmpCell;
}

void DisposeDeque(Deque D)
{
    while (!IsEmptyDeque(D))
        Pop(D);
    free(D);
}

void MakeEmptyDeque(Deque D)
{
    D->Size = 0;
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
    if (IsEmptyDeque(D)) {
        D->Front = NewNode(X, NULL, NULL);
        D->Rear = D->Front;
    } else {
        D->Front->Previous = NewNode(X, NULL, D->Front);
        D->Front = D->Front->Previous;
    }
    D->Size++;
}

ElementType Pop(Deque D)
{
    Position P;
    ElementType res;

    if (IsEmptyDeque(D))
        Error("Pop: operate on empty deque");
    else {
        D->Size--;
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
    if (IsEmptyDeque(D)) {
        D->Front = NewNode(X, NULL, NULL);
        D->Rear = D->Front;
    } else {
        D->Rear->Next = NewNode(X, D->Rear, NULL);
        D->Rear = D->Rear->Next;
    }
    D->Size++;
}

ElementType Eject(Deque D)
{
    Position P;
    ElementType res;

    if (IsEmptyDeque(D))
        Error("Eject: operate on empty deque");
    else {
        D->Size--;
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
    if (IsEmptyDeque(D))
        Error("Front: empty Deque");
    else
        return D->Front->Element;
}

ElementType Rear(Deque D)
{
    if (IsEmptyDeque(D))
        Error("Rear: empty Deque");
    else
        return D->Rear->Element;
}

Position DequeFirst(Deque D)
{
    if (IsEmptyDeque(D))
        Error("DequeFirst: empty Deque");
    else
        return D->Front;
}

Position DequePrevious(Position P)
{
    return P->Previous;
}

Position DequeNext(Position P)
{
    return P->Next;
}

ElementType DequeValue(Position P)
{
    return P->Element;
}

/* Merge D2 before index Pos of D1 */
void DequeMerge(int Pos, Deque D1, Deque D2)
{
    Position P, Pre;

    D1->Size += D2->Size;
    if (IsEmptyDeque(D2))
        return;
    else if (IsEmptyDeque(D1)) {
        D1->Front = D2->Front;
        D1->Rear = D2->Rear;
        return;
    }

    P = D1->Front;
    for (P = D1->Front; P != NULL && Pos > 0; Pos--, P = P->Next)
        ;

    if (P == NULL) {
        D1->Rear->Next = D2->Front;
        D2->Front->Previous = D1->Rear;
        D1->Rear = D2->Rear;
        return;
    }

    Pre = P->Previous;
    if (Pre == NULL)
        D1->Front = D2->Front;
    else {
        Pre->Next = D2->Front;
        D2->Front->Previous = Pre;
    }
    P->Previous = D2->Rear;
    D2->Rear->Next = P;
}

void PrintDeque(Deque D)
{
    Position P;

    printf("{ ");
    for (P = D->Front; P != NULL; P = P->Next)
        printf("%d ", P->Element);
    printf("}\n");
}