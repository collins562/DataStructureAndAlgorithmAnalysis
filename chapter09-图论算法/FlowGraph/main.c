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
    AddVertex(G, "t");

    Adjoin(G, 0, 1, 3); // s->A
    Adjoin(G, 0, 2, 2); // s->B
    Adjoin(G, 1, 2, 1); // A->B
    Adjoin(G, 1, 3, 3); // A->C
    Adjoin(G, 1, 4, 4); // A->D
    Adjoin(G, 2, 4, 2); // B->D
    Adjoin(G, 3, 5, 2); // C->t
    Adjoin(G, 4, 5, 3); // D->t

    PrintFlowGraph(G);
    PrintFlowGraph(FindMaxFlow(G));
}

