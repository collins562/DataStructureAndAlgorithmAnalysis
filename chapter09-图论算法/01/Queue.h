struct QueueRecord;
struct Node;
typedef struct QueueRecord *Queue;
typedef struct Node *PtrToNode;
typedef PtrToNode Position;
typedef int ElementType;

int IsEmpty(Queue Q);
Queue CreateQueue(void);
void DisposeQueue(Queue Q);
void MakeEmpty(Queue Q);
void Enqueue(ElementType X, Queue Q);
ElementType Front(Queue Q);
void Dequeue(Queue Q);
ElementType FrontAndDequeue(Queue Q);
