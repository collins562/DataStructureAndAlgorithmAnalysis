#include <stdio.h>
#include "Graph.h"
#include "DisjSet.h"

int main()
{
    int i;
    Graph G;
    Table T;

    G = CreateGraph();
    AddVertex(G, "V1");
    AddVertex(G, "V2");
    AddVertex(G, "V3");
    AddVertex(G, "V4");
    AddVertex(G, "V5");
    AddVertex(G, "V6");
    AddVertex(G, "V7");

    BiAdjoin(G, 0, 1, 2);
    BiAdjoin(G, 1, 4, 10);
    BiAdjoin(G, 4, 6, 6);
    BiAdjoin(G, 6, 5, 1);
    BiAdjoin(G, 5, 2, 5);
    BiAdjoin(G, 2, 0, 4);
    BiAdjoin(G, 3, 0, 1);
    BiAdjoin(G, 3, 1, 3);
    BiAdjoin(G, 3, 4, 7); 
    BiAdjoin(G, 3, 6, 4);
    BiAdjoin(G, 3, 5, 8);
    BiAdjoin(G, 3, 2, 2);

    T = CreateTable(7);
    InitTable(0, G, T);
    PrintTable(T);

    Prim(T);
    PrintTable(T);

    Kruskal(G);
}

