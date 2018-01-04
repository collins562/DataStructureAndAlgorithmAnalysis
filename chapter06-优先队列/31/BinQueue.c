#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "BinQueue.h"

#define Capacity    255
#define MaxTrees    8
#define Infinity    10000
#define Error(Message) fprintf(stderr, "%s\n", Message)

struct BinNode {
    ElementType Element;
    Position    LeftChild;
    Position    NextSibling;
};

struct Collection {
    int     CurrentSize;
    BinTree TheTrees[MaxTrees];
};

BinQueue Initialize(void)
{
    int i;
    BinQueue H;

    H = malloc(sizeof(struct Collection));
    if (H == NULL)
        Error("Out of space");

    for (i = 0; i < MaxTrees; i++)
        H->TheTrees[i] = NULL;

    H->CurrentSize = 0;
    return H;
}

int IsEmpty(BinQueue H)
{
    return H->CurrentSize == 0;
}

static BinTree CombineTrees(BinTree T1, BinTree T2)
{
    if (T1->Element > T2->Element)
        return CombineTrees(T2, T1);
    T2->NextSibling = T1->LeftChild;
    T1->LeftChild = T2;
    return T1;
}

void Insert(ElementType X, BinQueue H)
{
    int i;
    BinTree T, Carry;

    if (H->CurrentSize == Capacity)
        Error("Insert would exceed capacity");

    T = malloc(sizeof(struct BinNode));
    if (T == NULL)
        Error("Out of space");

    T->Element = X;
    T->LeftChild = T->NextSibling = NULL;

    Carry = T;
    for (i = 0; i < MaxTrees; i++) {
        if (H->TheTrees[i] == NULL) {
            H->TheTrees[i] = Carry;
            break;
        } else {
            Carry = CombineTrees(H->TheTrees[i], Carry);
            H->TheTrees[i] = NULL;
        }
    }
    H->CurrentSize++;
}

/* Merge two binomial queues. Not optimized for early termination   */
/* H1 contains merges result                                        */
/* Just like bits addition between two char                         */
BinQueue Merge(BinQueue H1, BinQueue H2)
{
    BinTree T1, T2, Carry = NULL;
    int i, j;

    if (H1->CurrentSize + H2->CurrentSize > Capacity)
        Error("Merge would exceed capacity");

    H1->CurrentSize += H2->CurrentSize;
    for (i = 0, j = 1; j <= H1->CurrentSize; i++, j *= 2) {
        T1 = H1->TheTrees[i];
        T2 = H2->TheTrees[i];

        switch(!!T1 + 2 * !!T2 + 4 * !!Carry) {
            case 0: // No trees
            case 1: // Only H1
                break;
            case 2: // Only H2
                H1->TheTrees[i] = T2;
                H2->TheTrees[i] = NULL;
                break;
            case 4: // Only Carry
                H1->TheTrees[i] = Carry;
                Carry = NULL;
                break;
            case 3: // H1 and H2
                Carry = CombineTrees(T1, T2);
                H1->TheTrees[i] = H2->TheTrees[i] = NULL;
                break;
            case 5: // H1 and Carry
                Carry = CombineTrees(T1, Carry);
                H1->TheTrees[i] = NULL;
                break;
            case 6: // H2 and Carry
                Carry = CombineTrees(T2, Carry);
                H2->TheTrees[i] = NULL;
                break;
            case 7: // All three
                H1->TheTrees[i] = Carry;
                Carry = CombineTrees(T1, T2);
                H2->TheTrees[i] = NULL;
                break;
        }
    }
    return H1;
}

ElementType DeleteMin(BinQueue H)
{
    int i, j;
    int MinTree;            // The tree with the minimum item
    BinQueue DeletedQueue;
    Position DeletedTree, OldRoot;
    ElementType MinItem;

    if (IsEmpty(H)) {
        Error("Empty binimial queue");
        return -Infinity;
    }

    MinItem = Infinity;
    for (i = 0; i < MaxTrees; i++) {
        if (H->TheTrees[i] && H->TheTrees[i]->Element < MinItem) {
            // Update minimum
            MinItem = H->TheTrees[i]->Element;
            MinTree = i;
        }
    }

    DeletedTree = H->TheTrees[MinTree];
    OldRoot = DeletedTree;
    DeletedTree = DeletedTree->LeftChild;
    free(OldRoot);

    DeletedQueue = Initialize();
    DeletedQueue->CurrentSize = (1 << MinTree) - 1;
    for (j = MinTree - 1; j >= 0; j--) {
        DeletedQueue->TheTrees[j] = DeletedTree;
        DeletedTree = DeletedTree->NextSibling;
        DeletedQueue->TheTrees[j]->NextSibling = NULL;
    }

    H->TheTrees[MinTree] = NULL;
    H->CurrentSize -= DeletedQueue->CurrentSize + 1;

    Merge(H, DeletedQueue);
    return MinItem;
}

static void PrintBinTree(BinTree T)
{
    if (T != NULL) {
        PrintBinTree(T->NextSibling);
        printf("%d ", T->Element);
        PrintBinTree(T->LeftChild);
    }
}

void PrintBinQueue(BinQueue H)
{
    int i;

    for (i = 0; i < MaxTrees; i++)
        if (H->TheTrees[i] != NULL) {
            printf(" TREE %02d: ", i);
            PrintBinTree(H->TheTrees[i]);
            putchar('\n');
        }
    putchar('\n');
}
