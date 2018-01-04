#ifndef _RedBlackTree_H

struct RedBlackNode;
typedef struct RedBlackNode *Position;
typedef struct RedBlackNode *RedBlackTree;
typedef int ElementType;

RedBlackTree MakeEmpty(RedBlackTree T);
Position Top(RedBlackTree T);
RedBlackTree Find(ElementType X, RedBlackTree T);
RedBlackTree FindMin(RedBlackTree T);
RedBlackTree FindMax(RedBlackTree T);
RedBlackTree Initialize(void);
RedBlackTree Insert(ElementType X, RedBlackTree T);
RedBlackTree Remove(ElementType X, RedBlackTree T);
ElementType Retrieve(RedBlackTree T);
void PrintRedBlackTree(RedBlackTree T);

#endif