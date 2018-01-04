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

typedef int DistType;
typedef int CostType;

Graph CreateGraph(void);
void AddVertex(Graph G, VertexEltType Elt);
int IsAdjacent(Graph G, Vertex VerIndex, Vertex AdjIndex);
void Adjoin(Graph G, Vertex VerIndex, Vertex AdjIndex, CostType Cost);
void BiAdjoin(Graph G, Vertex V1, Vertex V2, CostType Cost);
void PrintGraph(Graph G);

struct TableStruct;
typedef struct TableStruct *Table;

Table CreateTable(int NumVertex);
void InitTable(Vertex Start, Graph G, Table T);
void ReadGraph(Graph G, Table T);
void PrintTable(Table T);
void PrintPath(Vertex V, Table T);
void Prim(Table T);
void Kruskal(Graph G);

struct EdgeStruct;
typedef struct EdgeStruct *Edge;

Edge CreateEdge(Vertex Start, Vertex End, CostType Cost);
int EdgeCostcmp(Edge E1, Edge E2);
void PrintEdge(Edge E);

#endif