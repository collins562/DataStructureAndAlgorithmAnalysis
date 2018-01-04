#ifndef _Graph_H

#ifndef MaxVertexNum
#define MaxVertexNum 50
#endif

struct GraphStruct;
struct VertexNode;
struct EdgeNode;
typedef struct GraphStruct *Graph;
typedef struct VertexNode *AdjList;
typedef struct VertexNode *VertexPos;
typedef struct EdgeNode *EdgePos;
typedef int Vertex;
typedef char *VertexEltType;

typedef int CapacityType;
typedef int FlowType;

Graph CreateGraph(void);
void AddVertex(Graph G, VertexEltType Elt);
int IsAdjacent(Graph G, Vertex VerIndex, Vertex AdjIndex);
void Adjoin(Graph G, Vertex VerIndex, Vertex AdjIndex, CapacityType Capacity);
EdgePos FindEdge(Vertex AdjIndex, VertexPos VP);
void PrintGraph(Graph G);

struct TableStruct;
typedef struct TableStruct *Table;

Table CreateTable(int NumVertex);
void InitTable(Vertex Start, Graph G, Table T);
void DisposeGraph(Graph G);
void ReadGraph(Graph G, Table T);
void PrintPath(Vertex V, Table T);
Graph RevoGraph(Graph G);
Graph FlowGraph(Graph G);
void FindPath(Graph G, Table T);
void RevokeFlowPath(Graph RG, Table T);
void AddFlowPath(Graph FG, Table T);
Graph FindMaxFlow(Graph G);
void PrintFlowGraph(Graph G);

#endif