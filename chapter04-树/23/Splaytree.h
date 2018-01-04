#ifndef _SplayTree_H

struct SplayNode;
typedef struct SplayNode *Position;
typedef struct SplayNode *SplayTree;
typedef int ElementType;

SplayTree MakeEmpty(SplayTree T);
Position Find(ElementType X, SplayTree T);
Position Splay(Position P);
Position SplayFind(ElementType X, SplayTree T);
Position FindMin(SplayTree T);
Position FindMax(SplayTree T);
SplayTree Insert(ElementType X, SplayTree T);
SplayTree Delete(ElementType X, SplayTree T);
ElementType Retrieve(Position P);
void Error(char *Message);
void PrintSplayTree(SplayTree T);

#endif