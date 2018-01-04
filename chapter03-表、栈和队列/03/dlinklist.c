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

struct Node {
    ElementType Element;
    Position    Previous;
    Position    Next;
};

int main()
{
    ElementType i;
    List L, P;
    L = MakeEmpty(L);
    P = MakeEmpty(P);
    for (i = 1; i < 11; i++)
        Insert(i, L, L);
    for (i = 7; i > 0; i=i-2)
        Insert(i, P, P);
    PrintLots(L, P);
}

List MakeEmpty(List L)
{
    L = (List) malloc(sizeof(List));
    // L->Element = 0;
    // L->Previous = NULL;
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

    for (P = L->Next; P != NULL && P->Element != X; P = P->Next)
        ;
    if (P != NULL) {
        TmpCell = P->Previous;
        TmpCell->Next = P->Next;
        P->Next->Previous = TmpCell;
        free(P);
    }
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
    TmpCell->Previous = P;
    TmpCell->Next = P->Next;
    P->Next = TmpCell;
    P->Next->Previous = TmpCell;
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

void Swap(ElementType X, List L)
{
    Position TmpCell;
    Position P = Find(X, L);
    if (P->Next == NULL)
        fprintf(stderr, "Swap: no element to swap\n");
    else {
        TmpCell = P->Next->Next;
        P->Previous->Next = P->Next;
        P->Next->Previous = P->Previous;
        P->Next->Next = P;
        P->Previous = P->Next;
        P->Next = TmpCell;
        TmpCell->Previous = P;
    }
}