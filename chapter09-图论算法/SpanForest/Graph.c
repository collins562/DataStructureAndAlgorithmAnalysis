#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "DisjSet.h"
#include "Graph.h"
#include "Queue.h"
#include "BinHeap.h"

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

struct EdgeStruct {
    Vertex   Start;
    Vertex   End;
    CostType Cost;
};

/* Vertices are numbered from 0 */
#define NotAVertex (-1)
#define Infiniity 10000
#define False     0
#define True      1

struct TableEntry {
    AdjList  Header; /* Adjacency list */
    int      Known;
    DistType Dist;
    Vertex   Path;
};

typedef struct TableEntry Cell;

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
        T->TheCells = malloc(sizeof(struct TableEntry) * NumVertex);
        if (T->TheCells == NULL)
            Error("Out of space");
    }
    return T;
}

void InitTable(Vertex Start, Graph G, Table T)
{
    int i;

    ReadGraph(G, T);
    for (i = 0; i < G->Size; i++) {
        T->TheCells[i].Known = False;
        T->TheCells[i].Dist  = Infiniity;
        T->TheCells[i].Path  = NotAVertex;
    }
    T->Start = Start;
    T->TheCells[Start].Dist = 0;
}

void ReadGraph(Graph G, Table T)
{
    Vertex V;

    if (G->Size > T->Size)
        Error("out of table range");
    for (V = 0; V < G->Size; V++) {
        printf("Read %s into Table\n", G->AdjLists[V].Element);
        T->TheCells[V].Header = G->AdjLists[V];
    }
}

void PrintPath1(Vertex V, Table T)
{
    if (T->TheCells[V].Path != NotAVertex) {
        PrintPath1(T->TheCells[V].Path, T);
        printf(" to ");
    }
    printf("%s", T->TheCells[V].Header.Element);
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

    printf("| Index | Known | Dist  | Path  |\n");
    for (V = 0; V < T->Size; V++) {
        C = T->TheCells[V];
        printf("| %5d | %5d | %5d | %5d |\n", V, C.Known, C.Dist, C.Path);
    }
    printf("\n");
}

Vertex MinVertex(Table T)
{
    Vertex i, V = NotAVertex;
    DistType MinD = Infiniity;

    for (i = 0; i < T->Size; i++)
        if (T->TheCells[i].Known == False && T->TheCells[i].Dist < MinD) {
            MinD = T->TheCells[i].Dist;
            V = i;
        }
    return V;
}

void Prim(Table T)
{
    Vertex V, W;
    EdgePos P;

    for ( ; ; ) { // O(|V|^2 |E|)
        V = MinVertex(T);       // O(|V|)
        if (V == NotAVertex)
            break;

        T->TheCells[V].Known = True;
        // O(|E| / |V|)
        for (P = T->TheCells[V].Header.FirstEdge; P != NULL; P = P->Next) {
            W = P->AdjIndex;
            if (!T->TheCells[W].Known)
                if (T->TheCells[W].Dist > P->Cost) {
                    T->TheCells[W].Dist = P->Cost;
                    T->TheCells[W].Path = V;
                }
        }
    }
}

void ReadGraphIntoHeapArray(Graph G, PriorityQueue H)
{
    Vertex V;
    EdgePos P;

    for (V = 0; V < G->Size; V++)
        for (P = G->AdjLists[V].FirstEdge; P != NULL; P = P->Next) {
            RawInsert(CreateEdge(V, P->AdjIndex, P->Cost), H);
            printf("Read Edge (%d->%d) into Heap\n", V, P->AdjIndex);
        }
}

void Kruskal(Graph G)
{
    int EdgesAccepted;
    DisjSet S;
    PriorityQueue H;
    SetType Uset, Vset;
    Edge E;

    InitSet(S);
    H = InitBinHeap(G->Size * G->Size);
    ReadGraphIntoHeapArray(G, H);
    BuildHeap(H);

    EdgesAccepted = 0;
    while (EdgesAccepted < G->Size - 1) {
        E = DeleteMin(H);
        /* Set can't take 0 as input */
        Uset = Find(E->Start + 1, S);
        Vset = Find(E->End + 1, S);
        if (Uset != Vset) {
            /* Accept the edge */
            printf("\tPick (%s->%s)\n", G->AdjLists[E->Start].Element,
                                      G->AdjLists[E->End].Element);
            EdgesAccepted++;
            SetUnion(S, Uset, Vset);
        }
    }
    Destroy(H);
}

Edge CreateEdge(Vertex Start, Vertex End, CostType Cost)
{
    Edge TmpCell;

    if ((TmpCell = malloc(sizeof(struct EdgeStruct))) == NULL)
        Error("Out of space");
    else {
        TmpCell->Start = Start;
        TmpCell->End = End;
        TmpCell->Cost = Cost;
    }
    return TmpCell;
}

int EdgeCostcmp(Edge E1, Edge E2)
{
    return E1->Cost - E2->Cost;
}

void PrintEdge(Edge E)
{
    printf("(%d, %d, %d)", E->Start, E->End, E->Cost);
}