#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "RedBlackTree.h"

#define Infinity    100000
#define NegInfinity -Infinity

typedef enum ColorType { Red, Black } ColorType;

struct RedBlackNode {
    ElementType  Element;
    RedBlackTree Left;
    RedBlackTree Right;
    ColorType    Color;
};

static Position NullNode = NULL;

void Error(char *Message);

RedBlackTree Initialize(void)
{
    RedBlackTree T;

    if (NullNode == NULL) {
        NullNode = malloc(sizeof(struct RedBlackNode));
        if (NullNode == NULL)
            Error("Out of space");
        NullNode->Left = NullNode->Right = NullNode;
        NullNode->Color = Black;
        NullNode->Element = Infinity;
    }

    T = malloc(sizeof(struct RedBlackNode));
    if (T == NULL)
        Error("Out of space");
    T->Element = NegInfinity;
    T->Left = T->Right = NullNode;
    T->Color = Black;
    return T;
}

RedBlackTree MakeEmpty(RedBlackTree T)
{
    return NullNode;
}

Position Top(RedBlackTree T)
{
    return T->Right;
}

Position Find(ElementType Item, RedBlackTree T)
{
    if (T == NullNode)
        return NullNode;
    else if (Item < T->Element)
        return Find(Item, T->Left);
    else if (Item > T->Element)
        return Find(Item, T->Right);
    else
        return T;
}

Position FindMin1(RedBlackTree T)
{
    if (T != NullNode)
        while (T->Left != NullNode)
            T = T->Left;
    return T;
}

Position FindMin(RedBlackTree T)
{
    if (T->Right == NullNode)
        Error("operate FindMin on empty tree");
    else
        return FindMin1(T->Right);
}

Position FindMax1(RedBlackTree T)
{
    if (T != NullNode)
        while (T->Right != NullNode)
            T = T->Right;
    return T;
}

Position FindMax(RedBlackTree T)
{
    if (T->Right == NullNode)
        Error("operate Findmax on empty tree");
    else
        return FindMax1(T->Right);
}

static Position SingleRotateWithLeft(Position K2)
{
    Position K1;

    K1 = K2->Left;
    K2->Left = K1->Right;
    K1->Right = K2;

    return K1; /* New root */
}

static Position SingleRotateWithRight(Position K2)
{
    Position K1;

    K1 = K2->Right;
    K2->Right = K1->Left;
    K1->Left = K2;

    return K1;
}

static Position Rotate(ElementType Item, Position Parent)
{
    if (Item < Parent->Element)
        return Parent->Left = Item < Parent->Left->Element ?
            SingleRotateWithLeft(Parent->Left) :
            SingleRotateWithRight(Parent->Left);
    else
        return Parent->Right = Item < Parent->Right->Element ?
            SingleRotateWithLeft(Parent->Right) :
            SingleRotateWithRight(Parent->Right);
}

static Position S, X, P, GP, GGP;

static void HandleInsertReorient(ElementType Item, RedBlackTree T)
{
    X->Color = Red;
    X->Left->Color = X->Right->Color = Black;

    if (P->Color == Red) {
        GP->Color = Red;
        if ((Item < GP->Element) != (Item < P->Element))
            P = Rotate(Item, GP);
        X = Rotate(Item, GGP);
        X->Color = Black;
    }
    T->Right->Color = Black;
}

RedBlackTree Insert(ElementType Item, RedBlackTree T)
{
    X = P = GP = T;
    NullNode->Element = Item;
    while (X->Element != Item) {
        GGP = GP; GP = P; P = X;
        if (Item < X->Element)
            X = X->Left;
        else
            X = X->Right;
        if (X->Left->Color == Red && X->Right->Color == Red)
            HandleInsertReorient(Item, T);
    }

    if (X != NullNode)   // Duplicate
        return NullNode;

    X = malloc(sizeof(struct RedBlackNode));
    if (X == NULL)
        Error("Out of space");
    X->Element = Item;
    X->Left = X->Right = NullNode;

    if (Item < P->Element)  // Attach to its parent
        P->Left = X;
    else
        P->Right = X;
    HandleInsertReorient(Item, T);  // Color red; maybe rotate

    return T;
}

static void UpdateRemovePtr(ElementType Item)
{
    GP = P; P = X;
    if (Item < X->Element) {
        S = X->Right;
        X = X->Left;
    } else {
        S = X->Left;
        X = X->Right;
    }
}

static void HandleRemoveReorient(ElementType Item, RedBlackTree T)
{
    Position NextX, NextS;

    if (S == NullNode)
        return;
    if (X->Left->Color == Black && X->Right->Color == Black) {
        P->Color = Black;
        X->Color = Red;
        if (S->Left->Color == Black && S->Right->Color == Black) {
            S->Color = Red;
        } else {    // then S is not NullNode
            if (S->Element > P->Element && S->Left->Color == Red)
                S = Rotate(S->Left->Element, P);
            else if (S->Element < P->Element && S->Right->Color == Red)
                S = Rotate(S->Right->Element, P);
            else
                S->Color = Red;
            GP = Rotate(S->Element, GP);         // rotate S to be the new root
        }
    } else {
        if (Item < X->Element) {
            NextX = X->Left;
            NextS = X->Right;
        } else {
            NextX = X->Right;
            NextS = X->Left;
        }
        if (NextX->Color == Black) {        // NextS->Color == Red
            GP = P;
            P = Rotate(NextS->Element, P);  // P = NextS (with color red)
            S = Item < P->Element ? P->Right : P->Left;
            P->Color = Black;
            X->Color = Red;
            HandleRemoveReorient(Item, T);
        }
    }
}

RedBlackTree Remove(ElementType Item, RedBlackTree T)
{
    ElementType RightMin;
    Position DeletePtr;

    X = P = GP = T;
    NullNode->Element = Item;
    if (T->Right->Left->Color == Black && T->Right->Right->Color == Black)
        T->Right->Color = Red; // paint red
    while (X->Element != Item) {
        UpdateRemovePtr(Item);
        if (P->Color == Red && X != NullNode)
            HandleRemoveReorient(Item, T);
    }

    if (X != NullNode) {
        if (X->Right == NullNode) {
            if (Item < P->Element)
                P->Left = X->Left;
            else
                P->Right = X->Left;
            free(X);
        } else {
            RightMin = FindMin(X)->Element;
            DeletePtr = X;   // store current X
            Remove(RightMin, X);
            DeletePtr->Element = RightMin;
        }
    }

    return T;
}

/* Get roots item */
ElementType Retrieve(RedBlackTree T)
{
    return T->Element;
}

void PrintRedBlackTree1(RedBlackTree T)
{
    printf("( ");
    if (T != NullNode) {
        printf("%d ", T->Element);
        if (T->Color == Red)
            printf("r ");
        else 
            printf("b ");
        PrintRedBlackTree1(T->Left);
        PrintRedBlackTree1(T->Right);
    }
    putchar(')');
}

void PrintRedBlackTree(RedBlackTree T)
{
    PrintRedBlackTree1(T->Right);
    printf("\n");
}

void Error(char *Message)
{
    fprintf(stderr, "%s\n", Message);
    exit(1);
}