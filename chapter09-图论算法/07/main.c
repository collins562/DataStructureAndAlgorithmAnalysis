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

    Adjoin(G, 0, 1, 5); // A
    Adjoin(G, 0, 2, 3);
    Adjoin(G, 1, 2, -12); // B
    Adjoin(G, 1, 4, 3);
    Adjoin(G, 1, 6, 1);
    Adjoin(G, 2, 3, 7); // C
    Adjoin(G, 2, 4, 7);
    Adjoin(G, 3, 0, 2); // D
    Adjoin(G, 3, 5, 6); 
    Adjoin(G, 4, 3, 2); // E
    Adjoin(G, 4, 5, 1);
    Adjoin(G, 6, 4, 1); // G

    PrintGraph(G);

    T = CreateTable(7);
    InitTable(0, G, T);
    WeightedNegative(T);

    PrintPath(1, T);
    PrintPath(2, T);
    PrintPath(3, T);
    PrintPath(4, T);
    PrintPath(5, T);
    PrintPath(6, T);

    InitTable(0, G, T);
    Dijkstra(T);

    PrintPath(1, T);
    PrintPath(2, T);
    PrintPath(3, T);
    PrintPath(4, T);
    PrintPath(5, T);
    PrintPath(6, T);
}

