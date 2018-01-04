#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "HashSep.h"
#include <math.h>

#define MinTableSize 6

struct ListNode {
    ElementType Element;
    Position    Next;
};

typedef Position List;

/* List *TheList will be an array of lists, allocated later         */
/* The lists use headers (for simplicity), though this wastes space */
struct HashTbl {
    int     TableSize;
    List    *TheLists;
};

static int IsPrime(int x)
{
    int i;

    if (x <= 1)
        return 0;

    for (i = 2; i <= sqrt(x); i++)
        if (x % i == 0)
            return 0;
    return 1;
}

static int NextPrime(int x)
{
    int i;

    for (i = x; !IsPrime(i); i++)
        ;
    return i;
}

static int Hash(int Key, int TableSize)
{
    return Key % TableSize;
}

HashTable InitializeTable(int TableSize)
{
    HashTable H;
    int i;

    if (TableSize < MinTableSize) {
        Error("InitializeTable: Table size too small");
        return NULL;
    }

    H = malloc(sizeof(struct HashTbl));
    if (H == NULL)
        Error("InitializeTable: Out of space");

    H->TableSize = NextPrime(TableSize);

    H->TheLists = malloc(sizeof(List) * H->TableSize);
    if (H->TheLists == NULL)
        Error("InitializeTable: out of space");

    for (i = 0; i < H->TableSize; i++) {
        H->TheLists[i] = malloc(sizeof(struct ListNode));
        if (H->TheLists[i] == NULL)
            Error("InitializeTable: Out of space");
        else
            H->TheLists[i]->Next = NULL;
    }

    return H;
}

Position Find(ElementType Key, HashTable H)
{
    Position P;
    List L;

    L = H->TheLists[Hash(Key, H->TableSize)];
    P = L->Next;
    while (P != NULL && P->Element != Key)
        P = P->Next;
    return P;
}

void Insert(ElementType Key, HashTable H)
{
    Position Pos, NewCell;
    List L;

    Pos = Find(Key, H);
    if (Pos == NULL) {
        NewCell = malloc(sizeof(struct ListNode));
        if (NewCell == NULL)
            Error("Insert: out of space");
        else {
            L = H->TheLists[Hash(Key, H->TableSize)];
            NewCell->Next = L->Next;
            NewCell->Element = Key;
            L->Next = NewCell;
        }
    }
}

void DestroyTable(HashTable H)
{
    int i;
    Position P, TmpCell;

    for (i = 0; i < H->TableSize; i++) {
        P = H->TheLists[i];
        do {
            TmpCell = P->Next;
            free(P);
            P = TmpCell;
        } while (P != NULL);
    }
    free(H);
}

void PrintHashTable(HashTable H)
{
    int i;
    Position P;

    printf("------------------\n");
    for (i = 0; i < H->TableSize; i++) {
        printf("%3d: ", i);
        P = H->TheLists[i]->Next;
        if (P != NULL) {
            while (P != NULL) {
                printf("%d ", P->Element);
                P = P->Next;
            }
            putchar('\n');
        } else
            printf("--\n");
    }
    printf("------------------\n");
}

void Error(char *Message)
{
    fprintf(stderr, "%s\n", Message);
    // exit(1);
}