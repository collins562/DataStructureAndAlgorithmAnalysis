#ifndef _BinQueue_H

struct BinNode;
struct Collection;
typedef struct BinNode *Position;
typedef struct BinNode *BinTree;
typedef struct Collection *BinQueue;
typedef int ElementType;

BinQueue Initialize(void);
int IsEmpty(BinQueue H);
void Insert(ElementType X, BinQueue H);
BinQueue Merge(BinQueue H1, BinQueue H2);
ElementType DeleteMin(BinQueue H);

void PrintBinQueue(BinQueue H);

#endif