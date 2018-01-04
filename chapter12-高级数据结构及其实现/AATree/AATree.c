#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define OK       1
#define Infinity 100000

struct AANode;
typedef struct AANode *AATree;
typedef struct AANode *Position;

typedef int ElementType;
typedef unsigned short USHORT;

AATree Initialize(void);
Position Find(ElementType Item, AATree T);
AATree Insert(ElementType Item, AATree T);
AATree Remove(ElementType Item, AATree T);
void PrintAATree(AATree T);

void test_aatree(void);
void print_help(void);
void process_args(char *first);
void process_input(void);
void process_error(void);

struct AANode {
    ElementType Element;
    AATree      Left;
    AATree      Right;
    USHORT      Level;
};

static Position NullNode = NULL;
static void Error(char *Message);

AATree Initialize(void)
{
    if (NullNode == NULL) {
        NullNode = malloc(sizeof(struct AANode));
        if (NullNode == NULL)
            Error("Out of space");
        NullNode->Left = NullNode->Right = NullNode;
        NullNode->Level = 0;
    }
    return NullNode;
}

static Position SingleRotateWithLeft(Position K2)
{
    Position K1;

    K1 = K2->Left;
    K2->Left = K1->Right;
    K1->Right = K2;

    return K1; /* New root */
}

static Position SingleRotateWithRight(Position K2)
{
    Position K1;

    K1 = K2->Right;
    K2->Right = K1->Left;
    K1->Left = K2;

    return K1;
}

/* if T's left child is on the same level as T, perform a rotation */
AATree Skew(AATree T)
{
    if (T->Left->Level == T->Level)
        T = SingleRotateWithLeft(T);
    return T;
}

AATree Split(AATree T)
{
    if (T->Right->Right->Level == T->Level) {
        T = SingleRotateWithRight(T);
        T->Level++;
    }
    return T;
}

Position Find(ElementType Item, AATree T)
{
    while (T != NullNode) {
        if (Item < T->Element)
            T = T->Left;
        else if (Item > T->Element)
            T = T->Right;
        else
            break;
    }
    return T;
}

AATree Insert(ElementType Item, AATree T)
{
    if (T == NullNode) {
        T = malloc(sizeof(struct AANode));
        if (T == NULL)
            Error("Out of space");
        else {
            T->Element = Item; T->Level = 1;
            T->Left = T->Right = NullNode;
        }
        return T;
    } else if (Item < T->Element)
        T->Left = Insert(Item, T->Left);
    else if (Item > T->Element)
        T->Right = Insert(Item, T->Right);

    T = Skew(T);
    T = Split(T);
    return T;
}

AATree Remove(ElementType Item, AATree T)
{
    static Position DeletePtr, LastPtr;

    if (T != NullNode) {
        // step 1: search down tree set lastptr and deleteptr
        LastPtr = T;
        if (Item < T->Element)
            T->Left = Remove(Item, T->Left);
        else {
            DeletePtr = T;
            T->Right = Remove(Item, T->Right);
        }

        // step 2: if at the bottom of the tree and item is present, 
        //         we remove it
        if (T == LastPtr) {
            if (DeletePtr != NullNode && Item == DeletePtr->Element) {
                DeletePtr->Element = T->Element;
                DeletePtr = NullNode;
                T = T->Right;
                free(LastPtr);
            }
        }
        // step 3: otherwise, we are not at the bottom; rebalance
        else if (T->Left->Level < T->Level - 1 ||
                 T->Right->Level < T->Level - 1) {
            if (T->Right->Level > --T->Level)
                T->Right->Level = T->Level;
            T = Skew(T);
            T->Right = Skew(T->Right);
            T->Right->Right = Skew(T->Right->Right);
            T = Split(T);
            T->Right = Split(T->Right);
        }
    }
    return T;
}

void PrintAATree1(AATree T)
{
    if (T != NullNode) {
        PrintAATree1(T->Left);
        printf("%d ", T->Element);
        PrintAATree1(T->Right);
    }
}

void PrintAATree(AATree T)
{
    printf("[ ");
    PrintAATree1(T);
    printf("]\n");
}

void Error(char *Message)
{
    fprintf(stderr, "%s\n", Message);
    exit(1);
}

void test_aatree(void)
{
    int i;
    AATree T;

    srand(time(NULL));
    T = Initialize();
    for (i = 0; i < 20; i++)
        T = Insert(rand() % 200, T);

    PrintAATree(T);
    for (i = 0; i < 100; i++)
        if (Find(i, T) != NullNode) {
            printf("Delete %d...\n", i);
            T = Remove(i, T);
        }
    PrintAATree(T);
}

/* for user interface */
void print_help(void)
{  
    puts("\nAATree implementation.\n");
    puts(" AATree [/h]");
    puts(" AATree [/i]");
    puts(" AATree [/t]\n");
    puts("  /h\tdisplay help info");
    puts("  /i\tto input points");
    puts("  /t\ttest AATree implementation");
}

void process_args(char *first)
{
    if (strcmp(first, "/h") == 0 || strcmp(first, "/?") == 0)
        print_help();
    else if (strcmp(first, "/t") == 0)
        test_aatree();
    else if (strcmp(first, "/i") == 0)
        process_input();
    else
        process_error();
}

void process_input(void)
{

}

void process_error(void)
{
    fprintf(stderr, "%s\n", "Arguments Error\n");
    print_help();
}

int main(int argc, char *argv[])
{
    switch (argc) {
        case   1: print_help(); break;
        case   2: process_args(argv[1]); break;
        default : process_error();
    }
    return OK;
}