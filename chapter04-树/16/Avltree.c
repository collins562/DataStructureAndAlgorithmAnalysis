#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Avltree.h"

#define Max(x, y) ((x) > (y) ? (x) : (y))

struct AvlNode {
    ElementType Element;
    AvlTree     Left;
    AvlTree     Right;
    int         Height;
};

AvlTree MakeEmpty(AvlTree T)
{
    if (T != NULL) {
        MakeEmpty(T->Left);
        MakeEmpty(T->Right);
        free(T);
    }
    return NULL;
}

static Position NewNode(ElementType X, AvlTree left, AvlTree right, int height)
{
    Position TmpCell;

    if ((TmpCell = malloc(sizeof(struct AvlNode))) == NULL)
        Error("NewNode: out of space");

    TmpCell->Element = X;
    TmpCell->Left = left;
    TmpCell->Right = right;
    TmpCell->Height = height;

    return TmpCell;
}

static int Height(Position P)
{
    if (P == NULL)
        return -1;
    else
        return P->Height;
}

Position Find(ElementType X, AvlTree T)
{
    if (T == NULL)
        return NULL;
    if (X < T->Element)
        return Find(X, T->Left);
    else if (X > T->Element)
        return Find(X, T->Right);
    else return T;
}

Position FindMin(AvlTree T)
{
    if (T != NULL)
        while (T->Left != NULL)
            T = T->Left;

    return T;
}

Position FindMax(AvlTree T)
{
    if (T != NULL)
        while (T->Right != NULL)
            T = T->Right;

    return T;
}

static void UpdateHeight(Position P)
{
    P->Height = Max(Height(P->Left), Height(P->Right)) + 1;
}

/* This function can be called only if K2 has a left child */
/* Perform a ratate between a node (K2) and its left child */
/* Update heights, then return new root                    */
static Position SingleRotateWithLeft(Position K2)
{
    Position K1;

    K1 = K2->Left;
    K2->Left = K1->Right;
    K1->Right = K2;

    UpdateHeight(K2);
    UpdateHeight(K1);

    return K1; /* New root */
}

static Position SingleRotateWithRight(Position K2)
{
    Position K1;

    K1 = K2->Right;
    K2->Right = K1->Left;
    K1->Left = K2;

    UpdateHeight(K2);
    UpdateHeight(K1);

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

    /* efficient vision
    Position K1, K2;
    K1 = K3->Left; K2 = K1->Right;
    
    K1->Right = K2->Left;
    K2->Left = K1;
    K3->Left = K2->Right;
    K2->Right = K3;

    UpdateHeight(K1);
    UpdateHeight(K2);
    UpdateHeight(K3);

    return K2;
    */
}

static Position DoubleRotateWithRight(Position K3)
{
    K3->Right = SingleRotateWithLeft(K3->Right);

    return SingleRotateWithRight(K3);

    /* effcient vision
    Position K1, K2;
    K1 = K3->Right; K2 = K1->Left;

    K1->Left = K2->Right;
    K2->Right = K1;
    K3->Right = K2->Left;
    K2->Left = K3;

    UpdateHeight(K1);
    UpdateHeight(K2);
    UpdateHeight(K3);

    return K2;
    */
}

AvlTree Insert(ElementType X, AvlTree T)
{
    if (T == NULL)
        /* Create and return a one-node tree */
        T = NewNode(X, NULL, NULL, 0);
    else if (X < T->Element) {
        T->Left = Insert(X, T->Left);
        if (Height(T->Left) - Height(T->Right) == 2)
            if (X < T->Left->Element)
                T = SingleRotateWithLeft(T);
            else
                T = DoubleRotateWithLeft(T);
    } else if (X > T->Element) {
        T->Right = Insert(X, T->Right);
        if (Height(T->Right) - Height(T->Left) == 2)
            if (X > T->Right->Element)
                T = SingleRotateWithRight(T);
            else
                T = DoubleRotateWithRight(T);
    }
    /* else X is in the tree already; we'll do nothing */

    UpdateHeight(T);
    return T;
}

AvlTree Delete(ElementType X, AvlTree T)
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
    printf(" %d ", T->Element);
}

void PrintAvlTree(AvlTree T)
{
    printf("(");
    if (T != NULL) {
        PrintElement(T);
        PrintAvlTree(T->Left);
        PrintAvlTree(T->Right);
    }
    printf(")");
}

void Error(char *Message)
{
    fprintf(stderr, "%s\n", Message);
    // exit(1);
}