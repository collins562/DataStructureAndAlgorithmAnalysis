#include <stdio.h>
#include "Graph.h"

int main()
{
    int i;
    Graph G;
    Table T;

    G = CreateGraph();
    AddVertex(G, "s");
    AddVertex(G, "A");
    AddVertex(G, "B");
    AddVertex(G, "C");
    AddVertex(G, "D");
    AddVertex(G, "E");
    AddVertex(G, "F");
    AddVertex(G, "G");
    AddVertex(G, "H");
    AddVertex(G, "I");
    AddVertex(G, "t");

    Adjoin(G, 0, 1, 1); // s->A
    Adjoin(G, 0, 4, 4); // s->D
    Adjoin(G, 0, 7, 6); // s->G
    Adjoin(G, 1, 2, 2); // A->B
    Adjoin(G, 1, 5, 2); // A->E
    Adjoin(G, 2, 3, 2); // B->C
    Adjoin(G, 3,10, 4); // C->t
    Adjoin(G, 4, 1, 3); // D->A
    Adjoin(G, 4, 5, 3); // D->E
    Adjoin(G, 5, 3, 2); // E->C
    Adjoin(G, 5, 6, 3); // E->F
    Adjoin(G, 5, 9, 3); // E->I
    Adjoin(G, 6, 3, 1); // F->C
    Adjoin(G, 6,10, 3); // F->t
    Adjoin(G, 7, 4, 2); // G->D
    Adjoin(G, 7, 5, 1); // G->E
    Adjoin(G, 7, 8, 6); // G->H
    Adjoin(G, 8, 5, 2); // H->E
    Adjoin(G, 8, 9, 6); // H->I
    Adjoin(G, 9, 6, 1); // D->E
    Adjoin(G, 9,10, 4); // D->E

    PrintFlowGraph(G);
    PrintFlowGraph(FindMaxFlow(G));
}

