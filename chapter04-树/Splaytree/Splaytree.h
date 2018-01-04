#ifndef _SplayTree_H

struct SplayNode;
typedef struct SplayNode *Position;
typedef struct SplayNode *SplayTree;
typedef int ElementType;

SplayTree Initialize(void);
SplayTree MakeEmpty(SplayTree T);
SplayTree Find(ElementType X, SplayTree T);
SplayTree FindMin(SplayTree T);
SplayTree FindMax(SplayTree T);
SplayTree Insert(ElementType X, SplayTree T);
SplayTree Delete(ElementType X, SplayTree T);
ElementType Retrieve(Position P);
void PrintSplayTree(SplayTree T);

#endif