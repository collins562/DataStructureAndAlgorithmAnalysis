#ifndef _SplayTree_H

struct SplayNode;
typedef struct SplayNode *SplayTree;
typedef int ElementType;

SplayTree MakeEmpty(SplayTree T);
SplayTree Find(ElementType X, SplayTree T);
SplayTree FindMin(SplayTree T);
SplayTree FindMax(SplayTree T);
SplayTree Initialize(void);
SplayTree Insert(ElementType X, SplayTree T);
SplayTree Remove(ElementType X, SplayTree T);
ElementType Retrieve(SplayTree T);
void PrintSplayTree(SplayTree T);

#endif