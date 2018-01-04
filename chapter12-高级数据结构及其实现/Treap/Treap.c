#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define OK       1
#define Infinity 100000

struct TreapNode;
typedef struct TreapNode *Treap;
typedef struct TreapNode *Position;

typedef int ElementType;
typedef unsigned int UINT;

Treap Initialize(void);
Position Find(ElementType Item, Treap T);
Treap Insert(ElementType Item, Treap T);
Treap Remove(ElementType Item, Treap T);
void PrintTreap(Treap T);

void test_aatree(void);
void print_help(void);
void process_args(char *first);
void process_input(void);
void process_error(void);

struct TreapNode {
    ElementType Element;
    Treap      Left;
    Treap      Right;
    UINT       Priority;
};

static Position NullNode = NULL;
static void Error(char *Message);

Treap Initialize(void)
{
    if (NullNode == NULL) {
        NullNode = malloc(sizeof(struct TreapNode));
        if (NullNode == NULL)
            Error("Out of space");
        NullNode->Left = NullNode->Right = NullNode;
        NullNode->Priority = Infinity;
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

Position Find(ElementType Item, Treap T)
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

Treap Insert(ElementType Item, Treap T)
{
    if (T == NullNode) {
        T = malloc(sizeof(struct TreapNode));
        if (T == NULL)
            Error("Out of space");
        else {
            T->Element = Item; T->Priority = rand();
            T->Left = T->Right = NullNode;
        }
    } else if (Item < T->Element) {
        T->Left = Insert(Item, T->Left);
        if (T->Left->Priority < T->Priority)
            T = SingleRotateWithLeft(T);
    } else if (Item > T->Element) {
        T->Right = Insert(Item, T->Right);
        if (T->Right->Priority < T->Priority)
            T = SingleRotateWithRight(T);
    }

    return T;
}

Treap Remove(ElementType Item, Treap T)
{
    if (T != NullNode) {
        if (Item < T->Element)
            T->Left = Remove(Item, T->Left);
        else if (Item > T->Element)
            T->Right = Remove(Item, T->Right);
        else {
            if (T->Left->Priority < T->Right->Priority)
                T = SingleRotateWithLeft(T);
            else
                T = SingleRotateWithRight(T);

            if (T != NullNode)
                T = Remove(Item, T);
            else {
                free(T->Left);
                T->Left = NullNode;
            }
        }
    }
    return T;
}

void PrintTreap1(Treap T)
{
    if (T != NullNode) {
        PrintTreap1(T->Left);
        printf("%d ", T->Element);
        PrintTreap1(T->Right);
    }
}

void PrintTreap(Treap T)
{
    printf("[ ");
    PrintTreap1(T);
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
    Treap T;

    srand(time(NULL));
    T = Initialize();
    for (i = 0; i < 20; i++)
        T = Insert(rand() % 200, T);

    PrintTreap(T);
    for (i = 0; i < 100; i++)
        if (Find(i, T) != NullNode) {
            printf("Delete %d...\n", i);
            T = Remove(i, T);
        }
    PrintTreap(T);
}

/* for user interface */
void print_help(void)
{  
    puts("\nAATree implementation.\n");
    puts(" Treap [/h]");
    puts(" Treap [/i]");
    puts(" Treap [/t]\n");
    puts("  /h\tdisplay help info");
    puts("  /i\tto input points");
    puts("  /t\ttest Treap implementation");
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