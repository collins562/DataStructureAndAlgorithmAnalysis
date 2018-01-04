struct QueueRecord;
struct Node;
typedef struct QueueRecord *Queue;
typedef struct Node *PtrToNode;
typedef PtrToNode Position;
typedef int ElementType;

int IsEmptyQueue(Queue Q);
Queue CreateQueue(void);
void DisposeQueue(Queue Q);
void MakeEmptyQueue(Queue Q);
void Enqueue(ElementType X, Queue Q);
ElementType Front(Queue Q);
ElementType Dequeue(Queue Q);
ElementType FrontAndDequeue(Queue Q);
int QueueIndex(ElementType Elt, Queue Q);
