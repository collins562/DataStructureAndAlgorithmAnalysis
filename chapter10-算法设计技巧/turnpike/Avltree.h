#define Max(x, y) ((x) > (y) ? (x) : (y))

struct AvlNode;
typedef struct AvlNode *Position;
typedef struct AvlNode *AvlTree;
typedef int ElementType;

void Init_AvlTree(AvlTree T);
AvlTree MakeEmpty(AvlTree T);
int IsEmpty(AvlTree T);
Position Find(ElementType X, AvlTree T);
Position FindMin(AvlTree T);
Position FindMax(AvlTree T);
void Insert(ElementType X, AvlTree *Tp);
void Delete(ElementType X, AvlTree *Tp);
ElementType DeleteMax(AvlTree *Tp);
ElementType Retrieve(Position P);
void Error(char *Message);
void PrintAvlTree(AvlTree T);
ElementType Retrieve(Position P);

