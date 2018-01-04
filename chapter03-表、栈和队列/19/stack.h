#include <stdio.h>
#include <stdlib.h>

struct Node;
typedef struct Node *PtrToNode;
typedef PtrToNode Stack;
typedef int ElementType;

int IsEmpty(Stack S);
Stack CreateStack(void);
void DisposeStack(Stack S);
void MakeEmpty(Stack S);
void Push(ElementType X, Stack S);
ElementType Top(Stack S);
ElementType Pop(Stack S);
void Error(char *Message);

struct Node {
    ElementType Element;
    PtrToNode   Next;
};

int IsEmpty(Stack S)
{
    return S->Next == NULL;
}

Stack CreateStack(void)
{
    Stack S;

    S = malloc(sizeof(struct Node));
    if (S == NULL) {
        Error("CreateStack: out of space");
        exit(1);
    }

    S->Next = NULL;
    MakeEmpty(S);
    return S;
}

void MakeEmpty(Stack S)
{
    if (S == NULL)
        Error("Must use CreateStack first");
    else
        while (!IsEmpty(S))
            Pop(S);
}

void Push(ElementType X, Stack S)
{
    PtrToNode TmpCell;

    TmpCell = malloc(sizeof(struct Node));
    if (TmpCell == NULL)
        Error("Push: out of space");
    else {
        TmpCell->Element = X;
        TmpCell->Next = S->Next;
        S->Next = TmpCell;
    }
}

ElementType Top(Stack S)
{
    if (!IsEmpty(S))
        return S->Next->Element;
    Error("Top: empty stack");
    return 0;   /* return value used to avoid warning       */
}

ElementType Pop(Stack S)
{
    ElementType elt;
    PtrToNode FirstCell;

    if (IsEmpty(S))
        Error("Pop: empty stack");
    else {
        FirstCell = S->Next;
        S->Next = S->Next->Next;
        elt = FirstCell->Element;
        free(FirstCell);
        return elt;
    }
}

void Error(char *Message)
{
    fprintf(stderr, "%s\n", Message);
    //exit(1);
}