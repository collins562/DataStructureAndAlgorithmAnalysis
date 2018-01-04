#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "SplayTree.h"

struct SplayNode {
    ElementType Element;
    SplayTree   Left;
    SplayTree   Right;
};

typedef struct SplayNode *Position;
static Position NullNode = NULL;

void Error(char *Message);

SplayTree Initialize(void)
{
    if (NullNode == NULL) {
        NullNode = malloc(sizeof(struct SplayNode));
        if (NullNode == NULL)
            Error("Out of space");
        NullNode->Left = NullNode->Right = NullNode;
    }
    return NullNode;
}

SplayTree MakeEmpty(SplayTree T)
{
    return NullNode;
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

SplayTree Splay(ElementType Item, Position X)
{
    static struct SplayNode Header;
    Position LeftTreeMax, RightTreeMin;

    Header.Left = Header.Right = NullNode;
    LeftTreeMax = RightTreeMin = &Header;
    NullNode->Element = Item;

    while (Item != X->Element) {
        if (Item < X->Element) {
            if (Item < X->Left->Element)
                X = SingleRotateWithLeft(X);
            if (X->Left == NullNode)
                break;
            RightTreeMin->Left = X;
            RightTreeMin = X;
            X = X->Left;
        } else {
            if (Item > X->Right->Element)
                X = SingleRotateWithRight(X);
            if (X->Right == NullNode)
                break;
            LeftTreeMax->Right = X;
            LeftTreeMax = X;
            X = X->Right;
        }
    }

    LeftTreeMax->Right = X->Left;
    RightTreeMin->Left = X->Right;
    X->Left = Header.Right;
    X->Right = Header.Left;

    return X;
}

SplayTree Find(ElementType Item, SplayTree T)
{
    return Splay(Item, T);
}

SplayTree FindMin(SplayTree T)
{
    while (T->Left != NullNode)
        T = SingleRotateWithLeft(T);
    return T;
}

SplayTree FindMax(SplayTree T)
{
    while (T->Right != NullNode)
        T = SingleRotateWithRight(T);
    return T;
}

SplayTree Insert(ElementType Item, SplayTree T)
{
    static Position NewNode = NULL;

    if (NewNode == NULL) {
        NewNode = malloc(sizeof(struct SplayNode));
        if (NewNode == NULL)
            Error("Out of space");
    }
    NewNode->Element = Item;

    if (T == NullNode) {
        NewNode->Left = NewNode->Right = NullNode;
        T = NewNode;
    } else {
        T = Splay(Item, T);
        if (Item < T->Element) {
            NewNode->Left = T->Left;
            NewNode->Right = T;
            T->Left = NullNode;
            T = NewNode;
        } else if (T->Element < Item) {
            NewNode->Left = T;
            NewNode->Right = T->Right;
            T->Right = NullNode;
            T = NewNode;
        } else
            return T;
    }

    NewNode = NULL;    // So next insert will call malloc
    return T;
}

SplayTree Remove(ElementType Item, SplayTree T)
{
    Position NewTree;

    if (T != NullNode) {
        T = Splay(Item, T);
        if (Item == T->Element) {
            if (T->Left == NullNode)
                NewTree = T->Right;
            else {
                NewTree = T->Left;
                NewTree = Splay(Item, NewTree);
                NewTree->Right = T->Right;
            }
            free(T);
            T = NewTree;
        }
    }

    return T;
}

/* Get roots item */
ElementType Retrieve(SplayTree T)
{
    return T->Element;
}

void PrintSplayTree1(SplayTree T)
{
    if (T != NullNode) {
        PrintSplayTree1(T->Left);
        printf("%d ", T->Element);
        PrintSplayTree1(T->Right);
    }
}

void PrintSplayTree(SplayTree T)
{
    printf("[ ");
    PrintSplayTree1(T);
    printf("]\n");
}

void Error(char *Message)
{
    fprintf(stderr, "%s\n", Message);
    exit(1);
}