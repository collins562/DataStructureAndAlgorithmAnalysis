#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "Graph.h"
#include "Queue.h"

#define Error(Message) fprintf(stderr, "%s\n", Message)

struct EdgeNode {
    Vertex       AdjIndex;
    CapacityType Capacity;
    EdgePos      Next;
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
#define ZeroFlow  0
#define Infinity  100000
#define False     0
#define True      1

struct TableEntry {
    AdjList  Header; /* Adjacency list */
    int      Known;
    FlowType Flow;
    Vertex   Path;
};

typedef struct TableEntry Cell;

struct TableStruct {
    int      Size;
    Vertex   Start;
    Cell     *TheCells;
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
    VertexPos TmpCell;

    printf("Add Vertex: %s\n", Elt);
    if (G->Size == MaxVertexNum) {
        Error("exceed the max allowed Vertex number");
        return;
    }

    TmpCell = malloc(sizeof(struct VertexNode));
    if (TmpCell == NULL)
        Error("Out of space");
    else {
        /* Assume that there won't be any duplicate Elt in AdjLists */
        TmpCell->Element = Elt;
        TmpCell->FirstEdge = NULL;
        G->AdjLists[G->Size++] = TmpCell;
    }
}

/* return 1 if AdjIndex in G->AdjLists[VerIndex] else return 0 */
int IsAdjacent(Graph G, Vertex VerIndex, Vertex AdjIndex)
{
    EdgePos P = G->AdjLists[VerIndex]->FirstEdge;

    for ( ; P != NULL && P->AdjIndex != AdjIndex; P = P->Next)
        ;
    return P != NULL;
}

/* Adjoin N Vertices to VerIndex */
void Adjoin(Graph G, Vertex VerIndex, Vertex AdjIndex, CapacityType Capacity)
{
    EdgePos TmpCell;

    if (!IsAdjacent(G, VerIndex, AdjIndex)) {
        TmpCell = malloc(sizeof(struct EdgeNode));
        if (TmpCell == NULL)
            Error("Out of space");
        TmpCell->AdjIndex = AdjIndex;
        TmpCell->Capacity = Capacity;
        TmpCell->Next = G->AdjLists[VerIndex]->FirstEdge;
        G->AdjLists[VerIndex]->FirstEdge = TmpCell;
        printf("Adjoin %s to %s\n", G->AdjLists[VerIndex]->Element,
                                    G->AdjLists[AdjIndex]->Element);
    } /* else AdjIndex is already adjacent to VerIndex, do nothing */
}

EdgePos FindEdge(Vertex AdjIndex, VertexPos VP)
{
    EdgePos EP;

    for (EP = VP->FirstEdge; EP != NULL; EP = EP->Next)
        if (EP->AdjIndex == AdjIndex)
            break;
    return EP;
}

void PrintGraph(Graph G)
{
    Vertex i;
    EdgePos P;

    for (i = 0; i < G->Size; i++) {
        printf("%s", G->AdjLists[i]->Element);
        for (P = G->AdjLists[i]->FirstEdge; P != NULL; P = P->Next)
            printf("->%s", G->AdjLists[P->AdjIndex]->Element);
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
        T->TheCells[i].Flow  = ZeroFlow;
        T->TheCells[i].Path  = NotAVertex;
    }
    T->Start = Start;
    T->TheCells[Start].Flow = Infinity;
}

void DisposeGraph(Graph G)
{
    free(G);
}

void ReadGraph(Graph G, Table T)
{
    Vertex V;

    if (G->Size > T->Size)
        Error("out of table range");
    for (V = 0; V < G->Size; V++) {
        printf("Read %s into Table\n", G->AdjLists[V]->Element);
        T->TheCells[V].Header = G->AdjLists[V];
    }
}

void PrintPath1(Vertex V, Table T)
{
    if (T->TheCells[V].Path != NotAVertex) {
        PrintPath1(T->TheCells[V].Path, T);
        printf(" to ");
    }
    printf("%s", T->TheCells[V].Header->Element);
}

void PrintPath(Vertex V, Table T)
{
    PrintPath1(V, T);
    putchar('\n');
}

Graph RevoGraph(Graph G)
{
    Graph RG;
    Vertex V;
    EdgePos P;

    printf("\nCreate Revocation Graph.\n");
    RG = CreateGraph();
    for (V = 0; V < G->Size; V++)
        AddVertex(RG, G->AdjLists[V]->Element);
    for (V = 0; V < G->Size; V++)
        for (P = G->AdjLists[V]->FirstEdge; P != NULL; P = P->Next) {
            Adjoin(RG, V, P->AdjIndex, P->Capacity);
            Adjoin(RG, P->AdjIndex, V, 0); // revocation path
        }
    return RG;
}

Graph FlowGraph(Graph G)
{
    Graph FG;
    Vertex V;
    EdgePos P;

    printf("\nCreate Flow Graph.\n");
    FG = CreateGraph();
    for (V = 0; V < G->Size; V++)
        AddVertex(FG, G->AdjLists[V]->Element);
    for (V = 0; V < G->Size; V++)
        for (P = G->AdjLists[V]->FirstEdge; P != NULL; P = P->Next)
            Adjoin(FG, V, P->AdjIndex, 0);
    return FG;
}

void FindPath(Graph G, Table T)
{
    Vertex V, W;
    Queue Q;
    EdgePos P;

    /* Assume index 0 is the start */
    InitTable(0, G, T);

    Q = CreateQueue();
    Enqueue(T->Start, Q);

    while (!IsEmpty(Q)) {
        V = Dequeue(Q);
        T->TheCells[V].Known = True;
        for (P = T->TheCells[V].Header->FirstEdge; P != NULL; P = P->Next) {
            W = P->AdjIndex;
            if (P->Capacity <= T->TheCells[V].Flow) {
                if (P->Capacity > T->TheCells[W].Flow) {
                    T->TheCells[W].Flow = P->Capacity;
                    T->TheCells[W].Path = V;
                }
            } else if (T->TheCells[W].Flow < T->TheCells[V].Flow) {
                T->TheCells[W].Flow = T->TheCells[V].Flow;
                T->TheCells[W].Path = V;
            }
            if (T->TheCells[W].Flow != ZeroFlow && !T->TheCells[W].Known)
                // index W has flow && W not in queue
                Enqueue(W, Q);
        }
    }
    DisposeQueue(Q);
}

/* Revoke flowpath in G */
void RevokeFlowPath(Graph RG, Table T)
{
    Vertex V, PV;
    EdgePos P;
    FlowType F;

    V = RG->Size - 1;
    F = T->TheCells[V].Flow;
    while ((PV = T->TheCells[V].Path) != NotAVertex) {
        FindEdge(V, RG->AdjLists[PV])->Capacity -= F;
        FindEdge(PV, RG->AdjLists[V])->Capacity += F;
        V = PV;
    }
}

/* Add flowpath into G */
void AddFlowPath(Graph FG, Table T)
{
    Vertex V, PV;
    EdgePos P;
    FlowType F;

    V = FG->Size - 1;
    F = T->TheCells[V].Flow;
    while ((PV = T->TheCells[V].Path) != NotAVertex) {
        if ((P = FindEdge(V, FG->AdjLists[PV])) == NULL) {
            /* PV -> V is a revoked path           */
            /* Find Edge in the opposite direction */
            P = FindEdge(PV, FG->AdjLists[V]);
            P->Capacity -= F;
        } else
            P->Capacity += F;
        V = PV;
    }
}

Graph FindMaxFlow(Graph G)
{
    Vertex V;
    Table T;
    Graph RG = RevoGraph(G);
    Graph FG = FlowGraph(G);

    /* Assume index 0 is start && index G->Size - 1 is end */
    V = G->Size - 1;
    T = CreateTable(G->Size);
    FindPath(RG, T);
    while (T->TheCells[V].Flow != ZeroFlow) {
        printf("\nFounded Flow Path (%d): ", T->TheCells[V].Flow);
        PrintPath(V, T);
        AddFlowPath(FG, T);
        RevokeFlowPath(RG, T);
        FindPath(RG, T);
    }
    
    DisposeGraph(RG);
    return FG;
}

void PrintFlowGraph(Graph G)
{
    /* level reversal */
    Vertex i;
    EdgePos P;

    for (i = 0; i < G->Size; i++) {
        printf("%s", G->AdjLists[i]->Element);
        for (P = G->AdjLists[i]->FirstEdge; P != NULL; P = P->Next)
            printf("-|%d|->%s", P->Capacity, G->AdjLists[P->AdjIndex]->Element);
        putchar('\n');
    }
}