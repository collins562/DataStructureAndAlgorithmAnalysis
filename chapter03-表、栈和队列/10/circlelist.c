#include <stdio.h>
#include <stdlib.h>
#include "clist.h"

struct Node {
    ElementType Element;
    Position    Next;
    Position    Previous;
};

int Josephus(int, int);
Position UpdatePos(Position, CircleList);

int main()
{
    printf("Josephus(N: 5, M: 0): %d\n", Josephus(5, 0));
    printf("Josephus(N: 5, M: 1): %d\n", Josephus(5, 1));
}

int Josephus(int N, int M)
{
    int i;
    CircleList circle;
    Position P, Tmp;

    circle = MakeEmpty(NULL);
    for (i = N; i > 0; i--)
        Insert(i, circle, circle);

    P = First(circle);
    while (Length(circle) > 1) {
        for (i = M % Length(circle); i > 0; i--)
            P = UpdatePos(P, circle);

        Tmp = UpdatePos(P, circle);
        P->Previous->Next = P->Next;
        P->Next->Previous = P->Previous;
        free(P);
        P = Tmp;
        circle->Element -= 1;
    }

    return First(circle)->Element;
}

Position UpdatePos(Position P, CircleList CL)
{
    return (IsLast(P, CL)) ? First(CL) : P->Next;
}

CircleList MakeEmpty(CircleList CL)
{
    CircleList nCL;

    nCL = malloc(sizeof(struct Node));
    if (nCL == NULL) {
        fprintf(stderr, "MakeEmpty: no more space\n");
        exit(1);
    }

    nCL->Element = 0;
    nCL->Previous = nCL;
    nCL->Next = nCL;

    return nCL;
}

Position First(CircleList CL)
{
    return (IsEmpty(NULL, CL)) ? NULL : CL->Next;
}

Position Last(CircleList CL)
{
    return (IsEmpty(NULL, CL)) ? NULL : CL->Previous;
}

Position Next(Position P)
{
    if (P == NULL) {
        fprintf(stderr, "Next: operate on NULL\n");
        exit(1);
    } else
        return P->Next;
}

ElementType Retrieve(Position P)
{
    if (P == NULL) {
        fprintf(stderr, "Retrieve: operate on NULL\n");
        exit(1);
    } else
        return P->Element;
}

Position NewNode(ElementType elt, Position previous, Position next)
{
    Position P;

    P = malloc(sizeof(struct Node));
    if (P == NULL) {
        fprintf(stderr, "NewNode: no more space!\n");
        exit(1);
    }

    P->Element = elt;
    P->Previous = previous;
    P->Next = next;
    return P;
}

/* Return true if CL is empty                       */
int IsEmpty(Position P, CircleList CL)
{
    return CL->Element == 0;
}

/* Return true if P is the last position in CircleList CL   */
/* Parameter CL is unused in this implementation            */
int IsLast(Position P, CircleList CL)
{
    return P->Next == CL;
}

int Length(CircleList CL)
{
    return CL->Element;
}

/* Return position of X in CL; NULL if not found            */
Position Find(ElementType X, CircleList CL)
{
    Position P;

    P = First(CL);
    if (IsEmpty(P, CL))
        return NULL;
    while (P->Element != X)
        if (IsLast(P, CL))
            return NULL;
        else
            P = P->Next;
    return P;
}

/* Delete first occurrence of X from a CircleList;          */
void Delete(ElementType X, CircleList CL)
{
    Position P;

    if ((P = Find(X, CL)) != NULL) {
        P->Previous->Next = P->Next;
        P->Next->Previous = P->Previous;
        free(P);
        CL->Element -= 1;
    }
}

/* Insert (after legal position P)                          */
void Insert(ElementType X, CircleList CL, Position P)
{
    Position TmpCell;

    TmpCell = NewNode(X, P, P->Next);
    P->Next->Previous = TmpCell;
    P->Next = TmpCell;
    CL->Element += 1;
}

void DeleteCircleList(CircleList CL)
{
    Position P, Tmp;

    P = First(CL);

    CL->Element = 0;
    CL->Previous = CL;
    CL->Next = CL;
 
    if (P != NULL)
        P->Previous->Next = NULL;
    
    while (P != NULL) {
        Tmp = P->Next;
        free(P);
        P = Tmp;
    }
}

void PrintCircleList(CircleList CL)
{
    Position P;

    printf("[ ");
    if (!IsEmpty(P, CL))
        for (P = First(CL); !IsLast(P, CL); P = P->Next)
            printf("%d ", P->Element);
    printf("]\n");
}
