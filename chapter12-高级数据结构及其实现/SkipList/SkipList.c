#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define OK       1
#define Infinity 100000

struct SkipNode;
typedef struct SkipNode *SkipList;
typedef struct SkipNode *Position;
typedef int ElementType;

SkipList Initialize(void);
Position Find(ElementType Item, SkipList L);
static void PromoteMiddle(Position P);
SkipList Insert(ElementType Item, SkipList L);
void Delete(ElementType Item, SkipList L);
void PrintSkipList(SkipList L);

void test_skiplist(void);
void print_help();
void process_args(char *first);
void process_input(void);
void process_error(void);

struct SkipNode {
    ElementType Element;
    SkipList    Right;
    SkipList    Down;
};

static Position Bottom = NULL; // needs initialization
static Position Tail   = NULL; // needs initialization

static void Error(char *Message);

SkipList Initialize(void)
{
    SkipList L;

    if (Bottom == NULL) {
        Bottom = malloc(sizeof(struct SkipNode));
        if (Bottom == NULL)
            Error("Out of space");
        Bottom->Right = Bottom->Down = Bottom;

        Tail = malloc(sizeof(struct SkipNode));
        if (Tail == NULL)
            Error("Out of space");
        Tail->Element = Infinity;
        Tail->Right = Tail;
    }

    // create the header node
    L = malloc(sizeof(struct SkipNode));
    if (L == NULL)
        Error("Out of space");
    L->Element = Infinity;
    L->Right = Tail;
    L->Down = Bottom;

    return L;
}

/* return position of node containing Item or Bottom if not found */
Position Find(ElementType Item, SkipList L)
{
    Position Current = L;

    Bottom->Element = Item;
    while (Item != Current->Element)
        if (Item < Current->Element)
            Current = Current->Down;
        else
            Current = Current->Right;

    return Current;
}

SkipList Insert(ElementType Item, SkipList L)
{
    Position Current = L;
    Position NewNode;

    Bottom->Element = Item;
    while (Current != Bottom) {
        while (Item > Current->Element)
            Current = Current->Right;

        // if gap size is 3 or at bottom level
        // and must insert, then promote the middle element
        if (Current->Element > Current->Down->Right->Right->Element) {
            NewNode = malloc(sizeof(struct SkipNode));
            if (NewNode == NULL)
                Error("Out of space");
            NewNode->Right = Current->Right;
            NewNode->Down = Current->Down->Right->Right;
            Current->Right = NewNode;
            NewNode->Element = Current->Element;
            Current->Element = Current->Down->Right->Element;
        } else
            Current = Current->Down;
    }

    // raise height of DSL if necessary
    if (L->Right != Tail) {
        NewNode = malloc(sizeof(struct SkipNode));
        if (NewNode == NULL)
            Error("Out of space");
        NewNode->Down = L;
        NewNode->Right = Tail;
        NewNode->Element = Infinity;
        L = NewNode;
    }

    return L;
}

void Delete(ElementType Item, SkipList L)
{
    Position TmpCell, NewNode, Current, CurLeft;

    for (Current = CurLeft = L; Current != Bottom; Current = Current->Down) {
        // there are no element duplicates in the same column and there must
        // be a duplicate element in each following level as Item found
        while (Item > Current->Element) {
            CurLeft = Current;
            Current = Current->Right;
        }

        // bottom-up deletion
        // walk through all levels to check if there is any duplicate elt
        // in the same column (gap size is 0) OR if gap size exceeds 4
        Delete(Item, Current->Down); 

        if (Item == Current->Element) {
            TmpCell = Current->Right;
            Current->Right = TmpCell->Right;
            Current->Element = TmpCell->Element;
            free(TmpCell);
        }
        // 1st: gap size may become 0 at least after deletion
        // 2nd: become 6 at most after deletion at each level; ignore 
        //      gap with size 3 (which won't affect deletion) promote 
        //      3rd element in gap if gap size >= 4
        if (Current->Element == Current->Down->Element) {
            // undo promote
            TmpCell = CurLeft->Right;
            CurLeft->Right = TmpCell->Right;
            CurLeft->Element = TmpCell->Element;
            if (TmpCell != Tail)
                free(TmpCell);
        } else if (Current->Down != Bottom &&
                    Current->Element > 
                    Current->Down->Right->Right->Right->Element) {
            NewNode = malloc(sizeof(struct SkipNode));
            if (NewNode == NULL)
                Error("Out of space");
            NewNode->Right = Current->Right;
            NewNode->Down = Current->Down->Right->Right->Right;
            Current->Right = NewNode;
            NewNode->Element = Current->Element;
            Current->Element = Current->Down->Right->Right->Element;
        }
    }
}

void PrintSkipList(SkipList L)
{
    Position P1, P2;

    printf("+-----\n");
    for (P1 = L; P1 != Bottom; P1 = P1->Down) {
        printf("| ");
        for (P2 = P1; P2->Right != Tail; P2 = P2->Right)
            printf("%3d->", P2->Element);
        putchar('\n');
    }
    printf("+-----\n");
}

void Error(char *Message)
{
    fprintf(stderr, "%s\n", Message);
    exit(1);
}

void test_skiplist(void)
{
    int i;
    SkipList L;

    srand(time(NULL));
    L = Initialize();
    for (i = 0; i < 20; i++)
        L = Insert(rand() % 200, L);

    PrintSkipList(L);
    for (i = 0; i < 100; i++)
        if (Find(i, L) != Bottom) {
            printf("Delete %d...\n", i);
            Delete(i, L);
        }
    PrintSkipList(L);
}

/* for user interface */
void print_help()
{  
    puts("\nSkiplist implementation.\n");
    puts(" skiplist [/h]");
    puts(" skiplist [/i]");
    puts(" skiplist [/t]\n");
    puts("  /h\tdisplay help info");
    puts("  /i\tto input points");
    puts("  /t\ttest skiplist implementation");
}

void process_args(char *first)
{
    if (strcmp(first, "/h") == 0 || strcmp(first, "/?") == 0)
        print_help();
    else if (strcmp(first, "/t") == 0)
        test_skiplist();
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