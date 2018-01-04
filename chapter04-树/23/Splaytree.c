#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "SplayTree.h"

struct SplayNode {
    ElementType Element;
    SplayTree   Left;
    SplayTree   Right;
    SplayTree   Parent;
};

SplayTree MakeEmpty(SplayTree T)
{
    if (T != NULL) {
        MakeEmpty(T->Left);
        MakeEmpty(T->Right);
        free(T);
    }
    return NULL;
}

static Position NewNode(ElementType X, SplayTree left, SplayTree right, SplayTree parent)
{
    Position TmpCell;

    if ((TmpCell = malloc(sizeof(struct SplayNode))) == NULL)
        Error("NewNode: out of space");

    TmpCell->Element = X;
    TmpCell->Left = left;
    TmpCell->Right = right;
    TmpCell->Parent = parent;

    return TmpCell;
}

static int IsRoot(Position P)
{
    return P != NULL && P->Parent == NULL;
}

/* return 1 if P1 is P2's left child                */
static int IsLeftChild(Position P1, Position P2)
{
    return P2 != NULL && P2->Left == P1;
}

/* return 1 if P1 is P2's right child               */
static int IsRightChild(Position P1, Position P2)
{
    return P2 != NULL && P2->Right == P1;
}

static void ParentTransfer(Position target, Position origin)
{
    target->Parent = origin->Parent;
    if (!IsRoot(origin))
        if (IsLeftChild(origin, origin->Parent))
            origin->Parent->Left = target;
        else
            origin->Parent->Right = target;
}

/* This function can be called only if K2 has a left child */
/* Perform a ratate between a node (K2) and its left child */
/* Update heights, then return new root                    */
static Position SingleRotateWithLeft(Position K2)
{
    Position K1, P;

    K1 = K2->Left;
    K2->Left = K1->Right;
    if (K1->Right != NULL)
        K1->Right->Parent = K2;
    K1->Right = K2;

    ParentTransfer(K1, K2);
    K2->Parent = K1;

    return K1; /* New root */
}

static Position SingleRotateWithRight(Position K2)
{
    Position K1;

    K1 = K2->Right;
    K2->Right = K1->Left;
    if (K1->Left != NULL)
        K1->Left->Parent = K2;
    K1->Left = K2;

    ParentTransfer(K1, K2);
    K2->Parent = K1;

    return K1;
}

/* This function can be called only if K3 has a left child */
/* and K3's left child has a right child                   */
/* Do the left-right double rotation. Update heights, then */
/* return new root                                         */
static Position DoubleRotateWithLeft(Position K3)
{
    /* rotate between K1 and K2 */
    K3->Left = SingleRotateWithRight(K3->Left);

    /* rotate between K3 and K2 */
    return SingleRotateWithLeft(K3);
}

static Position DoubleRotateWithRight(Position K3)
{
    K3->Right = SingleRotateWithLeft(K3->Right);

    return SingleRotateWithRight(K3);
}

static Position ZigZigSplaywithLeft(Position K3)
{
    Position K1, K2;

    K2 = K3->Left;
    K1 = K2->Left;

    K3->Left = K2->Right;
    if (K2->Right != NULL)
        K2->Right->Parent = K3;
    K2->Right = K3;
    K2->Left = K1->Right;
    if (K1->Right != NULL)
        K1->Right->Parent = K2;
    K1->Right = K2;

    ParentTransfer(K1, K3);
    K2->Parent = K1;
    K3->Parent = K2;

    return K1;
}

static Position ZigZigSplaywithRight(Position K3)
{
    Position K1, K2;

    K2 = K3->Right;
    K1 = K2->Right;

    K3->Right = K2->Left;
    if (K2->Left != NULL)
        K2->Left->Parent = K3;
    K2->Left = K3;
    K2->Right = K1->Left;
    if (K1->Left != NULL)
        K1->Left->Parent = K2;
    K1->Left = K2;

    ParentTransfer(K1, K3);
    K2->Parent = K1;
    K3->Parent = K2;

    return K1;
}

Position Find(ElementType X, SplayTree T)
{
    Position P;

    if (T == NULL)
        return NULL;
    if (X < T->Element) {
        return P = Find(X, T->Left);
    }
    else if (X > T->Element)
        return Find(X, T->Right);
    else
        return T;
}

Position SplayFind(ElementType X, SplayTree T)
{
    return Splay(Find(X, T));
}

Position Splay(Position P)
{
    Position Prt;

    if (P == NULL)
        return NULL;

    while (!IsRoot(P)) {
        Prt = P->Parent;
        if (IsRoot(Prt))
            if (IsLeftChild(P, Prt))
                P = SingleRotateWithLeft(Prt);
            else
                P = SingleRotateWithRight(Prt);
        else if (IsLeftChild(P, Prt))
            if (IsRightChild(Prt, Prt->Parent))
                P = DoubleRotateWithRight(Prt->Parent);
            else
                P = ZigZigSplaywithLeft(Prt->Parent);
        else if (IsLeftChild(Prt, Prt->Parent))
            P = DoubleRotateWithLeft(Prt->Parent);
        else 
            P = ZigZigSplaywithRight(Prt->Parent);
    }

    return P;
}

Position FindMin(SplayTree T)
{
    if (T != NULL)
        while (T->Left != NULL)
            T = T->Left;

    return T;
}

Position FindMax(SplayTree T)
{
    if (T != NULL)
        while (T->Right != NULL)
            T = T->Right;

    return T;
}

SplayTree Insert(ElementType X, SplayTree T)
{
    Position CurrPos, PrevPos, TmpCell;

    if (T == NULL)
        return NewNode(X, NULL, NULL, NULL);

    CurrPos = T; PrevPos = NULL;
    while (CurrPos != NULL) {
        PrevPos = CurrPos;
        if (X < CurrPos->Element)
            CurrPos = CurrPos->Left;
        else if (X > CurrPos->Element)
            CurrPos = CurrPos->Right;
        else
            return T;
    }

    TmpCell = NewNode(X, NULL, NULL, PrevPos);
    if (X < PrevPos->Element)
        PrevPos->Left = TmpCell;
    else
        PrevPos->Right = TmpCell;

    return T;
}

SplayTree Delete(ElementType X, SplayTree T)
{
    Position TmpCell;

    if (T == NULL)
        Error("Element not found");
    else if (X < T->Element)
        T->Left = Delete(X, T->Left);
    else if (X > T->Element)
        T->Right = Delete(X, T->Right);
    else if (T->Left && T->Right) {
        /*Replace with smallest in right subtree */
        TmpCell = FindMin(T->Right);
        T->Element = TmpCell->Element;
        T->Right = Delete(T->Element, T->Right);
    } else {
        TmpCell = T;
        if (T->Left == NULL)
            T = T->Right;
        else if (T->Right == NULL)
            T = T->Left;
        free(TmpCell);
    }
    
    return T;
}

static void PrintElement(Position T)
{
    printf("%d ", T->Element);
}

void PrintSplayTree(SplayTree T)
{
    printf("(");
    if (T != NULL) {
        PrintElement(T);
        PrintSplayTree(T->Left);
        PrintSplayTree(T->Right);
    }
    printf(")");
}

void Error(char *Message)
{
    fprintf(stderr, "%s\n", Message);
    // exit(1);
}