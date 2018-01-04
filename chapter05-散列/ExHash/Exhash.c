#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "ExHash.h"
#include <math.h>

#define MaxLeafSize 4  // Size of Leaf
#define MinBitSize  4  // Size of key bit

struct HashEntry {
    int         Depth;
    int         EltNum;
    ElementType Elements[MaxLeafSize];
};

struct Point {
    int TablePos;
    int LeafPos;
};

typedef struct HashEntry *Leaf;

/* Leaf *TheLeaves will be an array of HashEntry cells, allocated later */
struct HashTbl {
    int     BitSize;
    int     Depth;
    Leaf    *TheLeaves;
};

static Leaf NewLeaf(int Depth)
{
    Leaf NewLeaf;

    NewLeaf = malloc(sizeof(struct HashEntry));
    if (NewLeaf == NULL)
        Error("NewLeaf: out of space");

    NewLeaf->Depth  = Depth;
    NewLeaf->EltNum = 0;

    return NewLeaf;
}

/* Extend table when New Depth of Leaf is bigger than H's Depth         */
static void ExtendTable(int LeafNewDepth, Position Pos, HashTable H)
{
    int i, j;
    int ExTimes, OldSize;
    Leaf L, *OldLeaves;

    L = H->TheLeaves[Pos.TablePos];
    ExTimes = 1 << (LeafNewDepth - H->Depth);      // Table extend times
    OldSize = 1 << H->Depth;
    H->Depth = LeafNewDepth;
    OldLeaves = H->TheLeaves;
    H->TheLeaves = malloc(ExTimes * OldSize * sizeof(Leaf));
    
    for (i = 0; i < OldSize; i++)
        for (j = i*ExTimes; j < i*ExTimes+ExTimes; j++)
            H->TheLeaves[j] = OldLeaves[i];

    for (i = 0; i < (1 << (LeafNewDepth - L->Depth)); i++)
        H->TheLeaves[i+Pos.TablePos*ExTimes] = NewLeaf(LeafNewDepth);

    free(OldLeaves);
}

/* Extend Leaves inside H when new depth of Leaf <= H's Depth           */
static void ExtendLeaves(int LeafNewDepth, Position Pos, HashTable H)
{
    int i, j, step;
    int ExTimes, DepthDiff;
    Leaf L;

    L = H->TheLeaves[Pos.TablePos];
    ExTimes = 1 << (LeafNewDepth - L->Depth);       // Leaf extend times
    DepthDiff = H->Depth - L->Depth;                // Depth diff. between H & L
    
    step = 1 << (H->Depth - LeafNewDepth);
    for (i = Pos.TablePos; i < Pos.TablePos + (1 << DepthDiff); i += step) {
        H->TheLeaves[i] = NewLeaf(LeafNewDepth);
        for (j = i+1; j < i+ExTimes; j++)
            H->TheLeaves[j] = H->TheLeaves[i];
    }
}

/* Extend the leaf when the amount of its elements hits MaxLeafSize     */
/* Pos represent the Position of Table where will be extended           */
static void Extend(Position Pos, HashTable H)
{
    int i;
    int LNewDepth, DepthDiff;
    ElementType elt;
    Leaf L, L2;

    L = H->TheLeaves[Pos.TablePos];
    DepthDiff = H->Depth - L->Depth;
    Pos.TablePos = Pos.TablePos >> DepthDiff << DepthDiff;  // Find 1st place
    LNewDepth = L->Depth + 1;            // Depth increases by 1 in this case

    // H->TheLeaves[Pos.TablePos] will be extend into depth LNewDepth 
    // (all extended leaves); HashTable's TheLeaves might be extended
    // into LNewDepth also.
    if (LNewDepth > H->Depth)
        ExtendTable(LNewDepth, Pos, H);
    else
        ExtendLeaves(LNewDepth, Pos, H);

    for (i = 0; i < 4; i++) {
        elt = L->Elements[i];
        L2 = H->TheLeaves[elt >> (H->BitSize-LNewDepth) << (H->Depth-LNewDepth)];
        L2->Elements[L2->EltNum++] = elt;
    }

    free(L);
}

HashTable InitializeTable(int BitSize)
{
    HashTable H;
    int i;

    if (BitSize < MinBitSize)
        Error("InitializeTable: Key BitSize too small");

    H = malloc(sizeof(struct HashTbl));
    if (H == NULL)
        Error("InitializeTable: Out of space");

    H->BitSize = BitSize;
    // initialize HashTable with 2 branches: "0" and "1"
    H->Depth = 1;
    H->TheLeaves = malloc(sizeof(Leaf) * 2);
    if (H->TheLeaves == NULL)
        Error("InitializeTable: out of space");

    for (i = 0; i <= 1; i++)
        H->TheLeaves[i] = NewLeaf(1);

    return H;
}

Position Find(ElementType Key, HashTable H)
{
    Leaf L;
    Position CurrentPos;
    int i;

    if (Key >> H->BitSize > 0)
        Error("Key too big");
    else {
        CurrentPos.TablePos = Key >> (H->BitSize - H->Depth);
        CurrentPos.LeafPos  = -1;

        L = H->TheLeaves[CurrentPos.TablePos];
        for (i = 0; i < L->EltNum; i++)
            if (Key > L->Elements[i])
                break;
            else if (Key == L->Elements[i]) {
                CurrentPos.LeafPos = i;
                break;
            }
    }
    return CurrentPos;
}

void Insert(ElementType Key, HashTable H)
{
    int i, j;
    Leaf L;
    Position Pos;


    Pos = Find(Key, H);
    if (Pos.LeafPos != -1) // Key already exists, do nothing
        return;

    L = H->TheLeaves[Pos.TablePos];
    if (L->EltNum == 4) {
        Extend(Pos, H);
        Insert(Key, H);
    } else {
        // ensure a in-order sequence elements after insertion
        for (i = 0; i < L->EltNum; i++)
            if (L->Elements[i] > Key) {
                for (j = L->EltNum; j > i; j--)
                    L->Elements[j] = L->Elements[j-1];
                L->Elements[i] = Key;
                break;
            }

        if (L->Elements[i] != Key)
            L->Elements[L->EltNum] = Key;
        L->EltNum++;
    }
}

ElementType Retrieve(Position P, HashTable H)
{
    return H->TheLeaves[P.TablePos]->Elements[P.LeafPos];
}

void DestroyTable(HashTable H)
{
    free(H->TheLeaves);
    free(H);
}

static void PrintBits(int n, int bits)
{
    if (bits > 0) {
        PrintBits(n >> 1, bits-1);
        printf("%d", n % 2);
    }
}

static void PrintLeaf(Leaf L)
{
    int i;

    printf(" (Depth: %d) ", L->Depth);
    for (i = 0; i < L->EltNum; i++) {
        PrintBits(L->Elements[i], 8);
        putchar(' ');
    }
    putchar('\n');
}

void PrintHashTable(HashTable H)
{
    int i;
    Leaf L;

    printf("+------------------------------------------------------+\n");
    L = NULL;
    for (i = 0; i < (1 << H->Depth); i++) {
        printf("| ");
        PrintBits(i, H->Depth);
        printf(": ");
        if (L != H->TheLeaves[i] || L == NULL)
            PrintLeaf(H->TheLeaves[i]);
        else
            printf(" [POINT-TO-PREVIOUS]\n");
        L = H->TheLeaves[i];
    }
    printf("+------------------------------------------------------+\n");
    printf("(Depth: %d)\n", H->Depth);
}

void Error(char *Message)
{
    fprintf(stderr, "%s\n", Message);
    // exit(1);
}