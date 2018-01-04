#include <stdio.h>
#include "Graph.h"

int main()
{
    int i;
    Graph G;

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

    Adjoin(G, 0, 3, 1, 4, 7);
    Adjoin(G, 1, 1, 2);
    Adjoin(G, 2, 1, 3);
    Adjoin(G, 3, 1, 10);
    Adjoin(G, 4, 1, 5);
    Adjoin(G, 5, 3, 3, 6, 9);
    Adjoin(G, 6, 2, 3, 10);
    Adjoin(G, 7, 3, 4, 5, 8);
    Adjoin(G, 8, 2, 5, 9);
    Adjoin(G, 9, 2, 6, 10);

    PrintGraph(G);
    TopSort(G);
}

