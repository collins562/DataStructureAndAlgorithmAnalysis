#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "clist.h"

struct Node {
    ElementType Element;
    Position    Next;
    Position    Previous;
};

#define NUM 100

void rpermutation(int *, int);
void swap(int *, int *);
void Radixsort(int *, int, int);
int  RadixIndex(int, int);
void PrintArray(int *, int);

int main()
{
    int Array[NUM];

    rpermutation(Array, NUM);
    PrintArray(Array, NUM);
    Radixsort(Array, NUM, 3);
    PrintArray(Array, NUM);
}

void PArrays(CircleList *Array)
{
    int i;
    for (i = 0; i < 10; i++) {
        printf("(%d): ", Array[i]->Element);
        PrintCircleList(Array[i]);
    }
}

void Radixsort(int *NUMS, int n, int sorttimes)
{
    int i, j, index, power, length;
    CircleList Array[n];
    Position P, Tmp;

    for (i = 0; i < n; i++)
        Array[i] = MakeEmpty(NULL);

    for (i = 0; i < n; i++) {
        index = NUMS[i] % 10;
        Insert(NUMS[i], Array[index], Array[index]->Previous);
    }

    for (power = 1; power < sorttimes; power++) {
        for (i = 0; i < 10; i++) {
            if ((P = First(Array[i])) == NULL)
                continue;
            length = Length(Array[i]);
            for (j = 0; j < length; j++) {
                index = RadixIndex(P->Element, power);
                Tmp = P->Next;
                Transfer(P, Array[index]->Previous, Array[i], Array[index]);
                P = Tmp;
                if (RadixIndex(Last(Array[index])->Element, power-1) != 
                    RadixIndex(P->Element, power-1))
                    break;
            }
        }
    }

    j = 0;
    for (i = 0; i < 10; i++) {
        if ((P = First(Array[i])) == NULL)
            continue;
        while (P != Array[i]) {
            NUMS[j++] = P->Element;
            P = P->Next;
        }
    }
}

int RadixIndex(int n, int power)
{
    return (int) (n / pow(10, power)) % 10;
}

void rpermutation(int *A, int n)
{
    int i;
    for (int i = 0; i < n; i++)
        A[i] = i;
    for (int i = 0; i < n; i++)
        swap(&A[i], &A[rand() % (i+1)]);
}

void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

void PrintArray(int *Array, int n)
{
    int i;
    printf("[ ");
    for (i = 0; i < n; i++)
        printf("%d ", Array[i]);
    printf("]\n");
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

    if ((P = Find(X, CL)) != NULL)
        DeletePos(P, CL);
}

void DeletePos(Position P, CircleList CL)
{
    if (P != CL) {  // delete P if P is not header
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

void Transfer(Position P1, Position P2, CircleList CL1, CircleList CL2)
{
    if (P1 == CL1) {
        fprintf(stderr, "Transfer: P1 operate on header\n");
        exit(1);
    }

    P1->Previous->Next = P1->Next;
    P1->Next->Previous = P1->Previous;
    CL1->Element -= 1;

    P1->Next = P2->Next;
    P1->Previous = P2;
    P2->Next->Previous = P1;
    P2->Next = P1;
    CL2->Element += 1;
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
    int i;
    Position P;

    printf("[ ");
    P = First(CL);
    for (i = 0; i < Length(CL); i++) {
        printf("%d ", P->Element);
        P = P->Next;
    }
    printf("]\n");
}
