#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "Graph.h"
#include "Queue.h"

#define Min(X, Y)      ((X) < (Y)) ? (X) : (Y)
#define Error(Message) fprintf(stderr, "%s\n", Message)

struct EdgeNode {
    Vertex   AdjIndex;
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
void Adjoin(Graph G, Vertex VerIndex, Vertex AdjIndex)
{
    EdgePos TmpCell;

    if (!IsAdjacent(G, VerIndex, AdjIndex)) {
        TmpCell = malloc(sizeof(struct EdgeNode));
        if (TmpCell == NULL)
            Error("Out of space");
        TmpCell->AdjIndex = AdjIndex;
        TmpCell->Next = G->AdjLists[VerIndex].FirstEdge;
        G->AdjLists[VerIndex].FirstEdge = TmpCell;
        printf("Adjoin %s to %s\n", G->AdjLists[VerIndex].Element, 
                                    G->AdjLists[AdjIndex].Element);
    } /* else AdjIndex is already adjacent to VerIndex, do nothing */
}

void BiAdjoin(Graph G, Vertex V1, Vertex V2)
{
    Adjoin(G, V1, V2);
    Adjoin(G, V2, V1);
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

void PrintTable(Table T)
{
    Vertex V;
    Cell C;

    printf("|   | Visited | Num |\n");
    for (V = 0; V < T->Size; V++) {
        C = T->TheCells[V];
        printf("| %s | %7d | %3d |\n", C->Header.Element, C->Visited, C->Num);
    }
    printf("\n");
}

Graph ReverseGraph(Graph G)
{
    Vertex V;
    Graph CG;
    EdgePos P;

    printf("\nReverse Garph.\n");
    CG = CreateGraph();
    for (V = 0; V < G->Size; V++)
        AddVertex(CG, G->AdjLists[V].Element);
    for (V = 0; V < G->Size; V++)
        for (P = G->AdjLists[V].FirstEdge; P != NULL; P = P->Next)
            Adjoin(CG, P->AdjIndex, V);
    printf("Reverse Done.\n\n");
    return CG;
}

static int Counter;
static int VerticesCounter[MaxVertexNum];

void MarkVertices(Vertex V, Table T)
{
    Cell Vcell;
    Vertex W;
    EdgePos P;

    Vcell = T->TheCells[V];
    Vcell->Visited = True;
    for (P = Vcell->Header.FirstEdge; P != NULL; P = P->Next) {
        W = P->AdjIndex;
        if (!T->TheCells[W]->Visited)
            MarkVertices(W, T);
    }
    Vcell->Num = Counter;
    VerticesCounter[Counter++] = V;
    printf("Mark Vertex %s (Num: %d)\n", Vcell->Header.Element, Counter-1);
}

void FindStrongBranch1(Vertex V, Table T)
{
    EdgePos P;
    Cell Vcell;

    Vcell = T->TheCells[V];
    if (!Vcell->Visited) {
        Vcell->Visited = True;
        printf("%s->", Vcell->Header.Element);
        for (P = Vcell->Header.FirstEdge; P != NULL; P = P->Next)
            FindStrongBranch1(P->AdjIndex, T);
    }
}

void FindStrongBranch(Graph G)
{
    Table T;
    Graph RG;
    Vertex V, SCC, VCounter;

    T = CreateTable(G->Size);
    InitTable(0, G, T);

    Counter = 0;
    for (V = 0; V < G->Size; V++)
        if (!T->TheCells[V]->Visited)
            MarkVertices(V, T);

    RG = ReverseGraph(G);
    PrintGraph(RG);
    InitTable(0, RG, T);
    SCC = 0;
    for (VCounter = G->Size - 1; VCounter >= 0; VCounter--) {
        V = VerticesCounter[VCounter];
        if (!T->TheCells[V]->Visited) {
            printf("No.%d: ", ++SCC);
            FindStrongBranch1(V, T);
            putchar('\n');
        }
    }
}