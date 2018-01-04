#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

struct Node;
typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
typedef int ElementType;

List MakeEmpty(List L);
int IsEmpty(Position P, List L);
int IsLast(Position P, List L);
Position Find(ElementType X, List L);
void Delete(ElementType X, List L);
Position FindPrevious(ElementType X, List L);
void Insert(ElementType X, List L, Position P);
void DeleteList(List L);
void PrintList(List L);
Position Header(List L);
Position First(List L);
Position Advance(Position P);
ElementType Retrieve(Position P);

struct Node {
    ElementType Element;
    Position    Next;
};

int main()
{
    ElementType i;
    List L;
    MakeEmpty(L);
    for (i = 11; i < 20; i++)
        Insert(i, L, L);
    PrintList(L);
}

List MakeEmpty(List L)
{
    L->Element = 0;
    L->Next = NULL;
}

/* Return true if L is empty                        */
int IsEmpty(Position P, List L)
{
    return P->Next == NULL;
}

/* Return true if P is the last position in list L  */
/* Parameter L is unused in this implementation     */
int IsLast(Position P, List L)
{
    return P->Next == NULL;
}

/* Return position of X in L; NULL if not found     */
Position Find(ElementType X, List L)
{
    Position P;

    P = L->Next;
    while (P != NULL && P->Element != X)
        P = P->Next;
    return P;
}

/* Delete first occurrence of X from a list;        */
/* Assume use of a header node                      */
void Delete(ElementType X, List L)
{
    Position P, TmpCell;

    P = FindPrevious(X, L);

    if (!IsLast(P, L)) {
        TmpCell = P->Next;
        P->Next = TmpCell->Next;
        free(TmpCell);
    }
}

/* If x is not found, then Next field of returned   */
/* Position is NULL; Assume a header                */
Position FindPrevious(ElementType X, List L)
{
    Position P;

    P = L;
    while (P->Next != NULL && P->Next->Element != X)
        P = P->Next;

    return P;
}

/* Insert (after legal position P)                  */
/* Header implementation assumed                    */
/* Parameter L is unused in this implementation     */
void Insert(ElementType X, List L, Position P)
{
    Position TmpCell;

    TmpCell = malloc(sizeof(struct Node));
    if (TmpCell == NULL)
        fprintf(stderr, "Insert: out of space\n");

    TmpCell->Element = X;
    TmpCell->Next = P->Next;
    P->Next = TmpCell;
}

void DeleteList(List L)
{
    Position P, Tmp;

    P = L->Next;            /* header assumed       */
    L->Next = NULL;
    while (P != NULL) {
        Tmp = P->Next;
        free(P);
        P = Tmp;
    }
}

void PrintList(List L)
{
    Position P;

    printf("[ ");
    for (P = L->Next; P != NULL; P = P->Next)
        printf("%d ", P->Element);
    printf("]\n");
}