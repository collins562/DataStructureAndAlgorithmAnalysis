#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Huffman.h"
#include "BinHeap.h"

#define Error(Message) fprintf(stderr, "%s\n", Message)

struct TreeNode {
    // TreeNode to encode character
    char      C;
    PtrToTree Left;
    PtrToTree Right;
};

struct WeightTreeRecord {
    int       Weight;
    PtrToTree EncodeTree;
};

struct WeightPairRecord {
    int  Size;
    char C;
    int  Weight;
};

void InitCharHashTable(CharSetHashTable T)
{
    int i;

    for (i = 0; i < 128; i++)
        T[i] = 0;
}

WTree CreateWTree(char C, int Weight)
{
    WTree TmpCell;
    PtrToTree T;

    TmpCell = malloc(sizeof(struct WeightTreeRecord));
    if (TmpCell == NULL)
        Error("Out of space");
    else if ((T = malloc(sizeof(struct TreeNode))) == NULL)
        Error("Out of space");
    else {
        T->C = C;
        T->Left = T->Right = NULL;
        TmpCell->Weight = Weight;
        TmpCell->EncodeTree = T;
    }
    return TmpCell;
}

void DisposeEncodeTree(PtrToTree T)
{
    if (T != NULL) {
        DisposeEncodeTree(T->Left);
        DisposeEncodeTree(T->Right);
        free(T);
    }
}

void DisposeWTree(WTree T)
{
    DisposeEncodeTree(T->EncodeTree);
    free(T);
}

WTree MergeWTrees(PriorityQueue H)
{
    WTree T1, T2;
    PtrToTree TmpCell;

    if (IsEmptyHeap(H)) {
        Error("Empty Queue");
        return NULL;
    }

    T1 = DeleteMin(H); 
    if (IsEmptyHeap(H))
        return T1;

    T2 = DeleteMin(H);
    TmpCell = malloc(sizeof(struct TreeNode));
    if (TmpCell == NULL) {
        Error("Out of space");
        return NULL;
    }

    TmpCell->C = '\0';
    if (T1->Weight < T2->Weight) {
        TmpCell->Left = T1->EncodeTree;
        TmpCell->Right = T2->EncodeTree;
    } else {
        TmpCell->Left = T2->EncodeTree;
        TmpCell->Right = T1->EncodeTree;
    }
    T1->EncodeTree = TmpCell;
    T1->Weight += T2->Weight;
    Insert(T1, H);
    free(T2);
    return MergeWTrees(H);
}

WTree Huffman(CharSetHashTable T)
{
    PriorityQueue H;
    int i;

    H = InitBinHeap(128);
    for (i = 0; i < 128; i++)
        if (T[i] > 0)
            RawInsert(CreateWTree(i, T[i]), H);
    BuildHeap(H);
    return MergeWTrees(H);
}

void PrintEncodeTree(PtrToTree T)
{
    printf("(");
    if (T != NULL) {
        if (isspace(T->C))
            printf("%d", T->C);
        else
            printf("%c", T->C);
        if (T->Left != NULL) {
            PrintEncodeTree(T->Left);
            PrintEncodeTree(T->Right);
        }
    }
    printf(")");
}

void PrintWTree(WTree T)
{
    printf("Weight %d:\n", T->Weight);
    PrintEncodeTree(T->EncodeTree);
    putchar('\n');
}

/* return <0 if T1 < T2; 0 if T1 == T2; >0 if T1 > T2 */
int WTreecmp(WTree T1, WTree T2)
{
    return T1->Weight - T2->Weight;
}
