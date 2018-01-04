#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "Graph.h"
#include "Queue.h"

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

int *Indegrees(Graph G)
{
    Vertex i, *IndegreeArray;
    EdgePos P;

    IndegreeArray = malloc(sizeof(int) * G->Size); /* G->Size skip 0 */
    if (IndegreeArray == NULL) {
        Error("Out of space");
        return NULL;
    }

    for (i = 0; i < G->Size; i++)
        IndegreeArray[i] = 0;
    for (i = 0; i < G->Size; i++) {
        for (P = G->AdjLists[i].FirstEdge; P != NULL; P = P->Next)
            IndegreeArray[P->AdjIndex]++;
    }
    return IndegreeArray;
}

void TopSort(Graph G)
{
    Queue Q;
    int Counter = 0;
    Vertex *IndegreeArray;
    Vertex i, VerIndex;
    EdgePos P;

    Q = CreateQueue();
    IndegreeArray = Indegrees(G);

    for (i = 0; i < G->Size; i++)
        if (IndegreeArray[i] == 0)
            Enqueue(i, Q);

    while (!IsEmpty(Q)) {
        VerIndex = FrontAndDequeue(Q);
        printf("%d ", VerIndex);
        Counter++;

        for (P = G->AdjLists[VerIndex].FirstEdge; P != NULL; P = P->Next)
            if ((--IndegreeArray[P->AdjIndex]) == 0)
                Enqueue(P->AdjIndex, Q);
    }
    if (Counter != G->Size)
        Error("Graph has a cycle");

    free(IndegreeArray);
    DisposeQueue(Q);
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

void Unweighted(Table T)
{
    Queue Q;
    Vertex V, W;
    EdgePos P;

    Q = CreateQueue();
    if (T->Start != NotAVertex)
        Enqueue(T->Start, Q);

    while (!IsEmpty(Q)) {
        V = Dequeue(Q);
        T->TheCells[V].Known = True; /* not really needed */

        for (P = T->TheCells[V].Header.FirstEdge; P != NULL; P = P->Next) {
            W = P->AdjIndex;
            if (T->TheCells[W].Dist == Infiniity) {
                T->TheCells[W].Dist = T->TheCells[V].Dist + 1;
                T->TheCells[W].Path = V;
                Enqueue(W, Q);
            }
        }
    }
    DisposeQueue(Q);
}

Vertex MinVertex(Table T)
{
    Vertex i, V = NotAVertex;
    DistType MinD = Infiniity;

    for (i = 0; i < MaxVertexNum; i++)
        if (T->TheCells[i].Known == False && T->TheCells[i].Dist < MinD) {
            MinD = T->TheCells[i].Dist;
            V = i;
        }
    return V;
}

void Dijkstra(Table T)
{
    Vertex V, W, i;
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
                if (T->TheCells[V].Dist + P->Cost < T->TheCells[W].Dist) {
                    T->TheCells[W].Dist = T->TheCells[V].Dist + P->Cost;
                    T->TheCells[W].Path = V;
                }
        }
    }
}

void WeightedNegative(Table T)
{
    Queue Q;
    Vertex V, W;
    EdgePos P;

    Q = CreateQueue();
    if (T->Start != NotAVertex)
        Enqueue(T->Start, Q);

    while (!IsEmpty(Q)) {
        V = Dequeue(Q);
        T->TheCells[V].Known = True;
        for (P = T->TheCells[V].Header.FirstEdge; P != NULL; P = P->Next) {
            W = P->AdjIndex;
            if (T->TheCells[V].Dist + P->Cost < T->TheCells[W].Dist) {
                T->TheCells[W].Dist = T->TheCells[V].Dist + P->Cost;
                T->TheCells[W].Path = V;
                if (!T->TheCells[W].Known)
                    Enqueue(W, Q);
            }
        }
    }
}
