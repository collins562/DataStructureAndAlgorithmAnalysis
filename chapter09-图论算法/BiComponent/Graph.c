#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "Graph.h"
#include "Queue.h"

#define Min(X, Y)      ((X) < (Y)) ? (X) : (Y)
#define Error(Message) fprintf(stderr, "%s\n", Message)

struct EdgeNode {
    Vertex   AdjIndex;
    CostType Cost;
    EdgePos  Next;
};

struct VertexNode {
    VertexEltType Element;
    EdgePos       FirstEdge;
};

struct GraphStruct {
    int     Size;
    AdjList AdjLists[MaxVertexNum];
};

/* Vertices are numbered from 0 */
#define NotAVertex (-1)
#define Infiniity 10000
#define False     0
#define True      1

struct TableEntry {
    AdjList Header; /* Adjacency list */
    int     Visited;
    int     Num;
    int     Low;
    Vertex  Parent;
};

typedef struct TableEntry *Cell;

struct TableStruct {
    int    Size;
    Vertex Start;
    Cell   *TheCells;
};

Graph CreateGraph(void)
{
    Graph G;

    if ((G = malloc(sizeof(struct GraphStruct))) == NULL)
        Error("Out of space");
    else
        G->Size = 0; /* Vertices are numbered from 0 */
    return G;
}

void AddVertex(Graph G, VertexEltType Elt)
{
    if (G->Size == MaxVertexNum)
        Error("exceed the max allowed Vertex number");
    else {
        /* Assume that there are no Element duplicate in AdjLists */
        G->AdjLists[G->Size].Element = Elt;
        G->AdjLists[G->Size++].FirstEdge = NULL;
    }
    printf("Add Vertex: %s\n", G->AdjLists[G->Size-1]);
}

/* return 1 if AdjIndex in G->AdjLists[VerIndex] else return 0 */
int IsAdjacent(Graph G, Vertex VerIndex, Vertex AdjIndex)
{
    EdgePos P = G->AdjLists[VerIndex].FirstEdge;

    for ( ; P != NULL && P->AdjIndex != AdjIndex; P = P->Next)
        ;
    return P != NULL;
}

/* Adjoin N Vertices to VerIndex */
void Adjoin(Graph G, Vertex VerIndex, Vertex AdjIndex, CostType Cost)
{
    EdgePos TmpCell;

    if (!IsAdjacent(G, VerIndex, AdjIndex)) {
        TmpCell = malloc(sizeof(struct EdgeNode));
        if (TmpCell == NULL)
            Error("Out of space");
        TmpCell->AdjIndex = AdjIndex;
        TmpCell->Cost = Cost;
        TmpCell->Next = G->AdjLists[VerIndex].FirstEdge;
        G->AdjLists[VerIndex].FirstEdge = TmpCell;
        printf("Adjoin %s to %s\n", G->AdjLists[VerIndex].Element, 
                                    G->AdjLists[AdjIndex].Element);
    } /* else AdjIndex is already adjacent to VerIndex, do nothing */
}

void BiAdjoin(Graph G, Vertex V1, Vertex V2, CostType Cost)
{
    Adjoin(G, V1, V2, Cost);
    Adjoin(G, V2, V1, Cost);
}

void PrintGraph(Graph G)
{
    Vertex i;
    EdgePos P;

    for (i = 0; i < G->Size; i++) {
        printf("%s", G->AdjLists[i].Element);
        for (P = G->AdjLists[i].FirstEdge; P != NULL; P = P->Next)
            printf("->%s", G->AdjLists[P->AdjIndex].Element);
        putchar('\n');
    }
}

Table CreateTable(int NumVertex)
{
    Table T;

    T = malloc(sizeof(struct TableStruct));
    if (T == NULL)
        Error("Out of space");
    else {
        T->Size = NumVertex;
        T->Start = NotAVertex;
        T->TheCells = malloc(sizeof(Cell) * NumVertex);
        if (T->TheCells == NULL)
            Error("Out of space");
    }
    return T;
}

void InitTable(Vertex Start, Graph G, Table T)
{
    int i;
    Cell TmpCell;

    for (i = 0; i < G->Size; i++) {
        if ((TmpCell = malloc(sizeof(struct TableEntry))) == NULL) {
            Error("Out of space");
            return;
        }
        TmpCell->Visited = False;
        TmpCell->Num     = 0;
        TmpCell->Low     = 0;
        TmpCell->Parent  = NotAVertex;
        T->TheCells[i] = TmpCell;
    }
    ReadGraph(G, T);
    T->Start = Start;
}

void ReadGraph(Graph G, Table T)
{
    Vertex V;

    if (G->Size > T->Size)
        Error("out of table range");
    for (V = 0; V < G->Size; V++) {
        printf("Read %s into Table\n", G->AdjLists[V].Element);
        T->TheCells[V]->Header = G->AdjLists[V];
    }
}

void PrintPath1(Vertex V, Table T)
{
    if (T->TheCells[V]->Parent != NotAVertex) {
        PrintPath1(T->TheCells[V]->Parent, T);
        printf(" to ");
    }
    printf("%s", T->TheCells[V]->Header.Element);
}

void PrintPath(Vertex V, Table T)
{
    PrintPath1(V, T);
    putchar('\n');
}

void PrintTable(Table T)
{
    Vertex V;
    Cell C;

    printf("|   | Visited | Num | Low | Parent |\n");
    for (V = 0; V < T->Size; V++) {
        C = T->TheCells[V];
        printf("| %s | %7d | %3d | %3d | %6d |\n", 
                C->Header.Element, C->Visited, C->Num, C->Low, C->Parent);
    }
    printf("\n");
}

static int Counter;
static Vertex VertexStack[MaxVertexNum];
Vertex *VPtr;

void PushVertex(Vertex V)
{
    *VPtr++ = V;
}

Vertex PopVertex(void)
{
    return *--VPtr;
}

Vertex TopVertex(void)
{
    return *(VPtr - 1);
}

void FindArt1(Vertex V, Table T)
{
    Cell Vcell, Wcell;
    Vertex W;
    EdgePos P;

    PushVertex(V);
    Vcell = T->TheCells[V];
    Vcell->Visited = True;
    Vcell->Low = Vcell->Num = Counter++;
    for (P = Vcell->Header.FirstEdge; P != NULL; P = P->Next) {
        W = P->AdjIndex;
        Wcell = T->TheCells[W];
        if (!Wcell->Visited) {
            Wcell->Parent = V;
            FindArt1(W, T);
            if (Wcell->Low >= Vcell->Num && Vcell->Num != 1) {
                printf("{ ");
                while (TopVertex() != V)
                    printf("%s ", T->TheCells[PopVertex()]->Header.Element);
                printf("}\n");
            }
            Vcell->Low = Min(Vcell->Low, Wcell->Low);
        } else if (Vcell->Parent != W)
            Vcell->Low = Min(Vcell->Low, Wcell->Num);
    }
}

void FindArt(Vertex Root, Graph G)
{
    Table T;

    T = CreateTable(G->Size);
    InitTable(Root, G, T);

    VPtr = VertexStack;
    Counter = 1;
    FindArt1(Root, T);
    PrintTable(T);
}