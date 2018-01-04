#include <stdio.h>
#include <stdlib.h>

typedef struct Node *PtrToNode;
typedef PtrToNode Term;
typedef PtrToNode Polynomial;   /* Nodes sorted by exponent     */

struct Node {
    int Coefficient;
    int Exponent;
    PtrToNode Next;
};

Term NewNode(int, int, Term);
Polynomial MakeEmpty(Polynomial);
void Insert(int, int, Polynomial, Term);
Polynomial Addpoly(Polynomial, Polynomial);
Polynomial Mulpoly(Polynomial, Polynomial);
Term Find(int, Polynomial);
int AddCoeff(Term, Term);
int MulCoeff(Term, Term);
int AddExp(Term, Term);
void PrintPoly(Polynomial);

int main()
{
    int i;
    int terms1[] = { 0, 1, 3 };
    int terms2[] = { 1, 3, 5 };
    Polynomial p1, p2;

    p1 = MakeEmpty(p1);
    p2 = MakeEmpty(p2);

    for (i = 0; i < sizeof(terms1)/sizeof(terms1[0]); i++)
        Insert(terms1[i], terms1[i], p1, p1);
    for (i = 0; i < sizeof(terms2)/sizeof(terms2[0]); i++)
        Insert(terms2[i], terms2[i], p2, p2);

    PrintPoly(p1);
    PrintPoly(p2);
    PrintPoly(Addpoly(p1, p2));
    PrintPoly(Mulpoly(p1, p2));
}

Polynomial MakeEmpty(Polynomial poly)
{
    Polynomial TmpCell;

    return TmpCell = NewNode(0, 0, NULL);
}

Term NewNode(int Coeff, int Exp, Term Next)
{
    Term TmpCell = malloc(sizeof(struct Node));

    if (TmpCell == NULL) {
        fprintf(stderr, "NewNode: no more space\n");
        exit(1);
    }

    TmpCell->Coefficient = Coeff;
    TmpCell->Exponent = Exp;
    TmpCell->Next = Next;
    return TmpCell;
}

void Insert(int Coeff, int Exp, Polynomial poly, Term P)
{
    Term TmpCell;

    P->Next = TmpCell = NewNode(Coeff, Exp, P->Next);
}

/* add polynomials                                  */
/* Assume Polynomial is in the order of Exponent    */
Polynomial Addpoly(Polynomial poly1, Polynomial poly2)
{
    Polynomial npoly;
    Term t, t1 = poly1->Next, t2 = poly2->Next, last;

    npoly = MakeEmpty(npoly);
    for (last = npoly; t1 != NULL && t2 != NULL; last = last->Next) {
        if (t1->Exponent == t2->Exponent) {
            Insert(AddCoeff(t1, t2), t1->Exponent, npoly, last);
            t1 = t1->Next;
            t2 = t2->Next;
        } else if (t1->Exponent > t2->Exponent) {
            Insert(t1->Coefficient, t1->Exponent, npoly, last);
            t1 = t1->Next;
        } else {
            Insert(t2->Coefficient, t2->Exponent, npoly, last);
            t2 = t2->Next;
        }
    }

    for (t = (t1 == NULL) ? t2 : t1; t != NULL; t = t->Next) {
        Insert(t->Coefficient, t->Exponent, npoly, last);
        last = last->Next;
    }

    return npoly;
}

Polynomial Mulpoly(Polynomial poly1, Polynomial poly2)
{
    int e;
    Polynomial npoly;
    Term t1, t2, tmp;

    npoly = MakeEmpty(npoly);
    for (t1 = poly1->Next; t1 != NULL; t1 = t1->Next)
        for (t2 = poly2->Next; t2 != NULL; t2 = t2->Next) {
            tmp = Find(e = AddExp(t1, t2), npoly);
            tmp->Coefficient += AddCoeff(t1, t2);
        }


    return npoly;
}

Term Find(int Exp, Polynomial poly)
{
    Term t, TmpCell;

    for (t = poly->Next; t != NULL && t->Exponent != Exp; t = t->Next)
        if (t->Next == NULL || Exp > t->Next->Exponent) {
            TmpCell = NewNode(0, Exp, t->Next);
            t->Next = TmpCell;
            return TmpCell;
        }

    if (t == NULL) {
        TmpCell = NewNode(0, Exp, poly->Next);
        poly->Next = TmpCell;
        return TmpCell;
    } else
        return t;
}

int AddCoeff(Term t1, Term t2)
{
    return t1->Coefficient + t2->Coefficient;
}

int MulCoeff(Term t1, Term t2)
{
    return t1->Coefficient * t2->Coefficient;
}

int AddExp(Term t1, Term t2)
{
    return t1->Exponent + t2->Exponent;
}

void PrintPoly(Polynomial poly)
{
    Term t;

    printf("[ ");
    for (t = poly->Next; t != NULL; t = t->Next)
        printf("(%d %d) ", t->Coefficient, t->Exponent);
    printf("]\n");
}