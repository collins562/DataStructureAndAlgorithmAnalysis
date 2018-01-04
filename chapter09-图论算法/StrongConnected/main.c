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
    AddVertex(G, "H");
    AddVertex(G, "I");
    AddVertex(G, "J");

    Adjoin(G, 0, 1);
    Adjoin(G, 0, 3);
    Adjoin(G, 1, 2);
    Adjoin(G, 1, 5);
    Adjoin(G, 2, 0);
    Adjoin(G, 2, 3);
    Adjoin(G, 2, 4);
    Adjoin(G, 3, 4);
    Adjoin(G, 5, 2);
    Adjoin(G, 6, 5);
    Adjoin(G, 6, 7);
    Adjoin(G, 7, 5);
    Adjoin(G, 7, 9);
    Adjoin(G, 8, 7);
    Adjoin(G, 9, 8);

    FindStrongBranch(G);
}

