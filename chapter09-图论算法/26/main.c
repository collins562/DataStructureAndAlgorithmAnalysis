#include <stdio.h>
#include "Graph.h"

int main()
{
    int i;
    Graph G;
    Table T;

    G = CreateGraph();
    AddVertex(G, "A");
    AddVertex(G, "B");
    AddVertex(G, "C");
    AddVertex(G, "D");
    AddVertex(G, "E");
    AddVertex(G, "F");
    AddVertex(G, "G");

    Adjoin(G, 0, 1);
    Adjoin(G, 0, 2);
    Adjoin(G, 1, 2);
    Adjoin(G, 1, 4);
    Adjoin(G, 1, 6);
    Adjoin(G, 2, 3);
    Adjoin(G, 2, 4);
    Adjoin(G, 3, 0);
    Adjoin(G, 3, 5);
    Adjoin(G, 4, 3);
    Adjoin(G, 4, 5);
    Adjoin(G, 6, 4);

    PrintGraph(G);
    FindStrongBranch(G);
}

