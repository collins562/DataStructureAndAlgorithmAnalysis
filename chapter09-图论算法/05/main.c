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

    Adjoin(G, 0, 2, 
                 1, 5, 
                 2, 3); // A
    Adjoin(G, 1, 3, 
                 2, 2,
                 4, 3,
                 6, 1); // B
    Adjoin(G, 2, 2, 
                 3, 7,
                 4, 7); // C
    Adjoin(G, 3, 2,
                 0, 2,
                 5, 6); // D
    Adjoin(G, 4, 2,
                 3, 2,
                 5, 1); // E
    Adjoin(G, 6, 1,
                 4, 1); // G

    PrintGraph(G);

    T = CreateTable(7);
    InitTable(0, G, T);
    Unweighted(T);

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

