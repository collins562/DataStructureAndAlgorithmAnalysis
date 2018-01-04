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

void Error(char *Message);

static Position NullNode = NULL;

SplayTree Initialize(void)
{
    if (NullNode == NULL) {
        NullNode = malloc(sizeof(struct SplayNode));
        if (NullNode == NULL)
            Error("Out of space");
        NullNode->Parent = NullNode->Left = NullNode->Right = NullNode;
    }
    return NullNode;
}

SplayTree MakeEmpty(SplayTree T)
{

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
    return P != NullNode && P->Parent == NullNode;
}

/* return 1 if P1 is P2's left child                */
static int IsLeftChild(Position P1, Position P2)
{
    return P2 != NullNode && P2->Left == P1;
}

/* return 1 if P1 is P2's right child               */
static int IsRightChild(Position P1, Position P2)
{
    return P2 != NullNode && P2->Right == P1;
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
    K2->Right->Parent = K3;
    K2->Right = K3;
    K2->Left = K1->Right;
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
    K2->Left->Parent = K3;
    K2->Left = K3;
    K2->Right = K1->Left;
    K1->Left->Parent = K2;
    K1->Left = K2;

    ParentTransfer(K1, K3);
    K2->Parent = K1;
    K3->Parent = K2;

    return K1;
}

/* bottom-up splay */
SplayTree Splay(Position P)
{
    Position Prt;

    if (P == NullNode)
        return P;

    while (!IsRoot(P)) {
        Prt = P->Parent;
        if (IsRoot(Prt)) {
            if (IsLeftChild(P, Prt))
                P = SingleRotateWithLeft(Prt);
            else
                P = SingleRotateWithRight(Prt);
        } else if (IsLeftChild(P, Prt)) {
            if (IsRightChild(Prt, Prt->Parent))
                P = DoubleRotateWithRight(Prt->Parent);
            else
                P = ZigZigSplaywithLeft(Prt->Parent);
        } else if (IsLeftChild(Prt, Prt->Parent))
            P = DoubleRotateWithLeft(Prt->Parent);
        else 
            P = ZigZigSplaywithRight(Prt->Parent);
    }

    return P;
}

SplayTree Find_Item(ElementType X, SplayTree T)
{
    Position Pre = T;

    NullNode->Element = X;
    while (T->Element != X) {
        Pre = T;
        T = T->Element > X ? T->Left : T->Right;
    }
    return T == NullNode ? Pre : T;
}

SplayTree Find(ElementType X, SplayTree T)
{
    return Splay(Find_Item(X, T));
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

SplayTree Insert(ElementType X, SplayTree T)
{
    static Position new = NULL;

    if (new == NULL) {
        new = malloc(sizeof(struct SplayNode));
        if (new == NULL)
            Error("Out of space");
    }
    new->Element = X;
    new->Parent = NullNode;

    if (T == NullNode) {
        new->Left = new->Right = NullNode;
    } else {
        T = Find(X, T);
        if (X < T->Element) {
            new->Left = T->Left;
            new->Right = T;
            T->Left->Parent = new;
            T->Parent = new;
            T->Left = NullNode;
        } else if (X > T->Element) {
            new->Left = T;
            new->Right = T->Right;
            T->Right->Parent = new;
            T->Parent = new;
            T->Right = NullNode;
        } else
            return T;
    }

    T = new;
    new = NULL;
    return T;
}

SplayTree Delete(ElementType X, SplayTree T)
{
    Position NewTree;

    if (T != NullNode) {
        T = Find(X, T);
        printf("Top Element: %d\n", T->Element);
        if (T->Element == X) {
            if (T->Left == NullNode) {
                NewTree = T->Right;
                NewTree->Parent = NullNode;
            } else {
                T->Left->Parent = NullNode;
                NewTree = Find(X, T->Left);
                NewTree->Right = T->Right;
            }
            free(T);
            T = NewTree;
        }
    }
    return NewTree;
}

ElementType Retrieve(SplayTree T)
{
    if (T != NullNode)
        return T->Element;
    else
        Error("Empty splay tree.");
}

static void PrintElement(Position T)
{
    printf("%d ", T->Element);
}

void PrintSplayTree1(SplayTree T)
{
    if (T != NullNode) {
        PrintSplayTree1(T->Left);
        PrintElement(T);
        PrintSplayTree1(T->Right);
    }
}

void PrintSplayTree(SplayTree T)
{
    printf("[ ");
    PrintSplayTree1(T);
    printf("] \n");
}

void Error(char *Message)
{
    fprintf(stderr, "%s\n", Message);
    exit(1);
}