#ifndef _Graph_H

#ifndef MaxVertexNum
#define MaxVertexNum 50
#endif

struct GraphStruct;
struct VertexNode;
struct EdgeNode;
typedef struct GraphStruct *Graph;
typedef struct VertexNode AdjList;
typedef struct EdgeNode *EdgePos;
typedef int Vertex;
typedef char *VertexEltType;

Graph CreateGraph(void);
void AddVertex(Graph G, VertexEltType Elt);
int IsAdjacent(Graph G, Vertex VerIndex, Vertex AdjIndex);
void Adjoin(Graph G, Vertex VerIndex, int N, ...);
int *Indegrees(Graph G);
void TopSort(Graph G);
void PrintGraph(Graph G);

typedef int DistType;
typedef int CostType;

struct TableStruct;
typedef struct TableStruct *Table;

Table CreateTable(int NumVertex);
void InitTable(Vertex Start, Graph G, Table T);
void ReadGraph(Graph G, Table T);
void PrintPath(Vertex V, Table T);
void Unweighted(Table T);
void Dijkstra(Table T);

#endif