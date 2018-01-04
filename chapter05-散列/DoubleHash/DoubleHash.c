#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "DoubleHash.h"
#include <math.h>

#define MinTableSize 6

enum KindOfEntry { Legitimate, Empty, Deleted };

struct HashEntry {
    ElementType      Element;
    enum KindOfEntry Info;
};

typedef struct HashEntry Cell;

/* Cell *TheCells will be an array of HashEntry cells, allocated later */
struct HashTbl {
    int     TableSize;
    Cell    *TheCells;
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

static int Hash2(int Key, int R)
{
    return Key - Key % R;
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

    H->TheCells = malloc(sizeof(Cell) * H->TableSize);
    if (H->TheCells == NULL)
        Error("InitializeTable: out of space");

    for (i = 0; i < H->TableSize; i++)
        H->TheCells[i].Info = Empty;

    return H;
}

Position Find(ElementType Key, HashTable H)
{
    Position CurrentPos;
    int CollisionNum;

    CollisionNum = 0;
    CurrentPos = Hash(Key, H->TableSize);
    while (H->TheCells[CurrentPos].Info != Empty &&
           H->TheCells[CurrentPos].Element != Key) {
        // h2(X) = 7 - (X mod 7)
        CurrentPos = Hash2(CurrentPos, 7);
        if (CurrentPos >= H->TableSize)
            CurrentPos -= H->TableSize;
    }
    return CurrentPos;
}

void Insert(ElementType Key, HashTable H)
{
    Position Pos;

    Pos = Find(Key, H);
    if (H->TheCells[Pos].Info != Legitimate) {
        H->TheCells[Pos].Info = Legitimate;
        H->TheCells[Pos].Element = Key;
    }
}

void DestroyTable(HashTable H)
{
    free(H->TheCells);
    free(H);
}

void PrintHashTable(HashTable H)
{
    int i;

    printf("+-------------+\n");
    for (i = 0; i < H->TableSize; i++) {
        printf("| %3d: ", i);
        if (H->TheCells[i].Info == Legitimate)
            printf("%6d |\n", H->TheCells[i].Element);
        else
            printf("    -- |\n");
    }
    printf("+-------------+\n");
}

void Error(char *Message)
{
    fprintf(stderr, "%s\n", Message);
    // exit(1);
}