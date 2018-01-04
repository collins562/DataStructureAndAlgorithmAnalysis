#include <stdio.h>
#include "Graph.h"

int main()
{
    Graph G;

    G = CreateGraph();
    AddVertex(G, "v1");
    AddVertex(G, "v2");
    AddVertex(G, "v3");
    AddVertex(G, "v4");
    AddVertex(G, "v5");
    AddVertex(G, "v6");
    AddVertex(G, "v7");

    Adjoin(G, 0, 3, 1, 2, 3);
    Adjoin(G, 1, 2, 3, 4);
    Adjoin(G, 2, 1, 5);
    Adjoin(G, 3, 3, 2, 5, 6);
    Adjoin(G, 4, 2, 3, 6);
    Adjoin(G, 6, 1, 5);

    PrintGraph(G);
}