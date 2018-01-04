#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "Graph.h"
#include "Deque.h"

#define Min(X, Y)      ((X) < (Y)) ? (X) : (Y)
#define Error(Message) fprintf(stderr, "%s\n", Message)

struct EdgeNode {
    Vertex   AdjIndex;
    EdgePos  Next;
};

struct VertexNode {
    int           Outdegree;
    VertexEltType Element;
    EdgePos       FirstEdge;
};

struct GraphStruct {
    int     Size;
    AdjList AdjLists[MaxVertexNum];
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
    AdjList *L;

    if (G->Size == MaxVertexNum)
        Error("exceed the max allowed Vertex number");
    else {
        L = &G->AdjLists[G->Size++];
        /* Assume that there are no Element duplicate in AdjLists */
        L->Element = Elt;
        L->FirstEdge = NULL;
        L->Outdegree = 0;
    }
    printf("Add Vertex: %s\n", G->AdjLists[G->Size-1].Element);
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
    AdjList *L;

    if (!IsAdjacent(G, VerIndex, AdjIndex)) {
        TmpCell = malloc(sizeof(struct EdgeNode));
        if (TmpCell == NULL)
            Error("Out of space");

        L = &G->AdjLists[VerIndex];
        TmpCell->AdjIndex = AdjIndex;
        TmpCell->Next = L->FirstEdge;

        L->FirstEdge = TmpCell;
        L->Outdegree++;

        printf("Adjoin %s to %s\n", G->AdjLists[VerIndex].Element, 
                                    G->AdjLists[AdjIndex].Element);
    } /* else AdjIndex is already adjacent to VerIndex, do nothing */
}

/* Disjoin vertex VerIndex and vertex AdjIndex */
void Disjoin(Graph G, Vertex VerIndex, Vertex AdjIndex)
{
    EdgePos Previous, Current;

    Current = G->AdjLists[VerIndex].FirstEdge;
    if (Current == NULL)
        return;

    Previous = NULL;
    while (Current != NULL) {
        if (Current->AdjIndex == AdjIndex) {
            if (Previous == NULL)
                G->AdjLists[VerIndex].FirstEdge = Current->Next;
            else
                Previous->Next = Current->Next;
            G->AdjLists[VerIndex].Outdegree--;
            free(Current);
            break;
        }
        Previous = Current;
        Current = Current->Next;
    }
}

void BiAdjoin(Graph G, Vertex V1, Vertex V2)
{
    Adjoin(G, V1, V2);
    Adjoin(G, V2, V1);
}

void BiDisjoin(Graph G, Vertex V1, Vertex V2)
{
    Disjoin(G, V1, V2);
    Disjoin(G, V2, V1);
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

Graph CopyGraph(Graph G)
{
    Vertex V;
    Graph CG;
    EdgePos P;

    printf("\nCopy Graph.\n");
    CG = CreateGraph();
    for (V = 0; V < G->Size; V++)
        AddVertex(CG, G->AdjLists[V].Element);
    for (V = 0; V < G->Size; V++)
        for (P = G->AdjLists[V].FirstEdge; P != NULL; P = P->Next)
            Adjoin(CG, V, P->AdjIndex);

    printf("Copy done.\n\n");
    return CG;
}

int TotalDegrees(Graph G)
{
    Vertex V;
    int Degrees;

    for (V = Degrees = 0; V < G->Size; V++)
        Degrees += G->AdjLists[V].Outdegree;

    return Degrees;
}

void PathDFS(Vertex V, Deque Path, Graph G)
{
    Vertex W;

    /* Assume there will always be a circuit */
    Inject(V, Path);
    W = G->AdjLists[V].FirstEdge->AdjIndex;
    BiDisjoin(G, V, W);
    //PrintGraph(G);
    if (W != Front(Path))
        PathDFS(W, Path, G);
}

void FindEulerCircuit(Graph G)
{
    int Pos, D, TD;
    Position P;
    Deque Path, SubPath;
    Graph CG;

    CG = CopyGraph(G);
    Path = CreateDeque();
    SubPath = CreateDeque();

    Inject(0, Path);
    Pos = 0; P = DequeFirst(Path);
    while ((TD = TotalDegrees(CG)) > 0) {
        if (TD % 2 == 1) {
            Error("Euler Circuit Not Found.");
            return;
        }

        while (P != NULL) {
            D = CG->AdjLists[DequeValue(P)].Outdegree;
            if (D != 0) {
                PathDFS(DequeValue(P), SubPath, CG);
                putchar('\n');
                DequeMerge(Pos, Path, SubPath);
                MakeEmptyDeque(SubPath);
                break;
            }
            Pos++;
            P = DequeNext(P);
        }

        if (P == NULL) {
            // can't find an entry
            Error("Euler Circuit Not Found");
            return;
        }
    }

    printf("Euler Path: \n\t");
    while (!IsEmptyDeque(Path))
        printf("%s->", G->AdjLists[Pop(Path)].Element);
}