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

void Insert(ElementType X, BinQueue H)
{
    BinTree  SingleNode;
    BinQueue SingleTree;

    SingleTree = Initialize();
    SingleTree->CurrentSize = 1;

    SingleNode = malloc(sizeof(struct BinNode));
    if (SingleNode == NULL)
        Error("Out of space");

    SingleNode->Element = X;
    SingleNode->LeftChild = SingleNode->NextSibling = NULL;

    SingleTree->TheTrees[0] = SingleNode;
    Merge(H, SingleTree);
}

static BinTree CopyTree(BinTree T)
{
    BinTree CopyT;

    if (T != NULL) {
        CopyT = malloc(sizeof(struct BinNode));
        if (CopyT == NULL)
            Error("Out of space");

        CopyT->Element = T->Element;
        CopyT->LeftChild = CopyTree(T->LeftChild);
        CopyT->NextSibling = CopyTree(T->NextSibling);
        return CopyT;
    } else
        return NULL;
}

static BinTree CombineTrees(BinTree T1, BinTree T2)
{
    if (T1 == T2)
        T2 = CopyTree(T1);
    else if (T1->Element > T2->Element)
        return CombineTrees(T2, T1);
    T2->NextSibling = T1->LeftChild;
    T1->LeftChild = T2;
    return T1;
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

        // if H1 == H2: there are merely 3 possible cases: 3, 4, 7;
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
                if (H1 != H2)
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
