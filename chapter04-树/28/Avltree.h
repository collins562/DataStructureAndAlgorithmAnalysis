#ifndef _AvlTree_H

struct AvlNode;
typedef struct AvlNode *Position;
typedef struct AvlNode *AvlTree;
typedef int ElementType;

AvlTree MakeEmpty(AvlTree T);
Position Find(ElementType X, AvlTree T);
Position FindMin(AvlTree T);
Position FindMax(AvlTree T);
AvlTree Insert(ElementType X, AvlTree T);
AvlTree Delete(ElementType X, AvlTree T);
ElementType Retrieve(Position P);
void Error(char *Message);
void PrintAvlTree(AvlTree T);
int CountNodes(AvlTree T);
int CountLeaves(AvlTree T);
int CountFullNodes(AvlTree T);

#endif