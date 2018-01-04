#include <stdio.h>

typedef struct Node *PtrToNode;

struct Node {
    int Coefficient;
    int Exponent;
    PtrToNode Next;
};

typedef PtrToNode Polynomial;   /* Nodes sorted by exponent     */

void Insert(int, int, List, Position);
List Addpoly(List, List);

void Insert(int Coeff, int Exp, List L, Position P)
{
    Position TmpCell;

    TmpCell = malloc(sizeof(struct Node));
    if (TmpCell == NULL) {
        fprintf(stderr, "Insert: out of space!\n");
        exit(1);
    }

    TmpCell->Coefficient = Coeff;
    TmpCell->Exponent = Exp;
    TmpCell->Next = P->Next;
    P->Next = TmpCell;
}

/* add polynomials                          */
/* Assume List is in the order of Exponent  */
List Addpoly(List L1, List L2)
{
    List nL;
    Position p, p1 = L1->Next, p2 = L2->Next, last;

    nL = MakeEmpty(nL);
    for (last = nL, p1 != NULL && p2 != NULL; last = last->Next) {
        if (p1->Exponent == p2->Exponent) {
            Insert(p1->Coefficient+p2->Coefficient, p1->Exponent, nL, last);
            p1 = p1->Next;
            p2 = p2->Next;
        } else if (p1->Exponent < p2->Exponent) {
            Insert(p1->Coefficient, p1->Exponent, nL, last);
            p1 = p1->Next;
        } else {
            Insert(p2->Coefficient, p2->Exponent, nL, last);
            p2 = p2->Next;
        }
    }

    p = (p1 == NULL) ? p1 : p2;
    while (p != NULL) {
        Insert(p->Coefficient, p->Exponent, nL, last);
        p = p->Next;
        last = last->Next;
    }
    return nL;
}