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
void Adjoin(Graph G, Vertex VerIndex, Vertex AdjIndex);
void Disjoin(Graph G, Vertex VerIndex, Vertex AdjIndex);
void BiAdjoin(Graph G, Vertex V1, Vertex V2);
void PrintGraph(Graph G);
void FindEulerCircuit(Graph G);

#endif