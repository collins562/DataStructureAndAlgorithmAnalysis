#include <stdio.h>
#include "Graph.h"

int main()
{
    Graph G;

    G = CreateGraph();
    AddVertex(G, "A");
    AddVertex(G, "B");
    AddVertex(G, "C");
    AddVertex(G, "D");
    AddVertex(G, "E");
    AddVertex(G, "F");
    AddVertex(G, "G");
    AddVertex(G, "H");
    AddVertex(G, "I");
    AddVertex(G, "J");
    AddVertex(G, "K");
    AddVertex(G, "L");

    BiAdjoin(G, 0, 2);
    BiAdjoin(G, 2, 1);
    BiAdjoin(G, 1, 7);
    BiAdjoin(G, 7, 8);
    BiAdjoin(G, 8, 9);
    BiAdjoin(G, 8, 11);
    BiAdjoin(G, 11, 9);
    BiAdjoin(G, 9, 10);
    BiAdjoin(G, 9, 4);
    BiAdjoin(G, 9, 3);
    BiAdjoin(G, 3, 10);
    BiAdjoin(G, 3, 4);
    BiAdjoin(G, 3, 0);
    BiAdjoin(G, 3, 2);
    BiAdjoin(G, 2, 5);
    BiAdjoin(G, 2, 8);
    BiAdjoin(G, 5, 8);
    BiAdjoin(G, 6, 2);
    BiAdjoin(G, 6, 3);
    BiAdjoin(G, 6, 8);
    BiAdjoin(G, 6, 9);

    PrintGraph(G);
    FindEulerCircuit(G);
}

