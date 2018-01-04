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
void PrintLots(List L, List P);
Position Header(List L);
Position First(List L);
Position Advance(Position P);
ElementType Retrieve(Position P);

List Intersect(List L1, List L2);
List Union(List L1, List L2);

List Reverse1(List);
void Reverse2(List);

struct Node {
    ElementType Element;
    Position    Next;
};

int main()
{
    ElementType i;
    List L;
    L = MakeEmpty(NULL);
    for (i = 10; i > 0; i--)
        Insert(i, L, L);
    PrintList(L);
    PrintList(Reverse1(L));
    Reverse2(L);
    PrintList(L);
}

List MakeEmpty(List L)
{
    L = (List) malloc(sizeof(List));
    // L->Element = 0;
    L->Next = NULL;
    return L;
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

void PrintLots(List L, List P)
{
    int index;
    Position lp = L->Next, pp = P->Next;

    printf("[ ");
    for (index = 1; lp != NULL && pp != NULL; lp = lp->Next, index++)
        if (pp->Element == index) {
            printf("%d ", lp->Element);
            pp = pp->Next;
        }
    printf("]\n");
}

List Intersect(List L1, List L2)
{
    List nL;
    Position lp1 = L1->Next, lp2 = L2->Next, last;

    nL = MakeEmpty(nL);
    last = nL;
    while (lp1 != NULL && lp2 != NULL)
        if (lp1->Element == lp2->Element) {
            Insert(lp1->Element, nL, last);
            lp1 = lp1->Next;
            lp2 = lp2->Next;
            last = last->Next;
        } else if (lp1->Element < lp2->Element)
            lp1 = lp1->Next;
        else
            lp2 = lp2->Next;
    return nL;
}

List Union(List L1, List L2)
{
    List nL;
    Position lp1 = L1->Next, lp2 = L2->Next, last;

    nL = MakeEmpty(nL);
    for (last = nL; lp1 != NULL && lp2 != NULL; last = last->Next)
        if (lp1->Element <= lp2->Element) {
            Insert(lp1->Element, nL, last);
            if (lp1->Element == lp2->Element)
                lp2 = lp2->Next;
            lp1 = lp1->Next;
        } else {
            Insert(lp2->Element, nL, last);
            lp2 = lp2->Next;
        }
    while (lp1 != NULL) {
        Insert(lp1->Element, nL, last);
        lp1 = lp1->Next;
        last = last->Next;
    }
    while (lp2 != NULL) {
        Insert(lp2->Element, nL, last);
        lp2 = lp2->Next;
        last = last->Next;
    }
    return nL;
}

List Reverse1(List L)
{
    List rL;
    Position P;

    rL = MakeEmpty(NULL);
    for (P = L->Next; P != NULL; P = P->Next)
        Insert(P->Element, rL, rL);
    return rL;
}

void Reverse2(List L)
{
    Position P1, P2, P3;

    P1 = P2 = L->Next;
    while ((P3 = P1->Next) != NULL) {
        P1->Next = P3->Next;
        P3->Next = P2;
        P2 = P3;
    }
    L->Next = P2;
}

/* Assuming no header and L is not empty */
List ReverseList(List L)
{
    Position CurrentPos, NextPos, PreviousPos;

    PreviousPos = NULL;
    CurrentPos = L;
    while ((NextPos = CurrentPos->Next) != NULL) {
        CurrentPos->Next = PreviousPos;
        PreviousPos = CurrentPos;
        CurrentPos = NextPos;
    }
    CurrentPos->Next = PreviousPos;
    return CurrentPos;
}