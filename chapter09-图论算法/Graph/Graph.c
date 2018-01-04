#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "Graph.h"

#define Error(Message) fprintf(stderr, "%s\n", Message)

struct EdgeNode {
    Vertex  AdjIndex;
    EdgePos Next;
};

struct VertexNode {
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

/* Adjoin N Vertices with VerIndex */
void Adjoin(Graph G, Vertex VerIndex, int N, ...)
{
    va_list ap;
    Vertex i, AdjIndex;
    EdgePos TmpCell;

    va_start(ap, N);
    for (i = 0; i < N; i++) {
        AdjIndex = va_arg(ap, int);
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
    va_end(ap);
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
