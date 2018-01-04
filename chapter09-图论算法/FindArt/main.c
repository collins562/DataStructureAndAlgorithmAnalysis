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

    BiAdjoin(G, 0, 1, 0);
    BiAdjoin(G, 1, 2, 0);
    BiAdjoin(G, 2, 3, 0);
    BiAdjoin(G, 3, 0, 0);
    BiAdjoin(G, 2, 6, 0);
    BiAdjoin(G, 3, 4, 0);
    BiAdjoin(G, 3, 5, 0);
    BiAdjoin(G, 4, 5, 0);

    FindArt(0, G);
}

