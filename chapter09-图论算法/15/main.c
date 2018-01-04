#include <stdio.h>
#include "Graph.h"
#include "DisjSet.h"

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
    AddVertex(G, "H");
    AddVertex(G, "I");
    AddVertex(G, "J");

    BiAdjoin(G, 0, 1, 3);
    BiAdjoin(G, 1, 2, 10);
    BiAdjoin(G, 2, 6, 1);
    BiAdjoin(G, 6, 9, 8);
    BiAdjoin(G, 9, 8, 7);
    BiAdjoin(G, 8, 7, 4);
    BiAdjoin(G, 7, 3, 6);
    BiAdjoin(G, 3, 0, 4);
    BiAdjoin(G, 4, 0, 4);
    BiAdjoin(G, 4, 1, 2);
    BiAdjoin(G, 4, 3, 5);
    BiAdjoin(G, 4, 5, 11);
    BiAdjoin(G, 4, 7, 2);
    BiAdjoin(G, 4, 8, 1);
    BiAdjoin(G, 5, 1, 3);
    BiAdjoin(G, 5, 2, 6);
    BiAdjoin(G, 5, 6, 2);
    BiAdjoin(G, 5, 8, 3);
    BiAdjoin(G, 5, 9, 11);

    T = CreateTable(10);
    InitTable(0, G, T);
    PrintTable(T);

    Prim(T);
    PrintTable(T);

    Kruskal(G);
}

