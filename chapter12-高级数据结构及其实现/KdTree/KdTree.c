#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define OK       1
#define KeySize  2

struct KdNode;
typedef struct KdNode *KdTree;
typedef struct KdNode *Position;

typedef unsigned int UINT;
typedef UINT ItemType[2];

KdTree Initialize(void);
Position FindMin(KdTree T, int Level);
KdTree DeleteMin(KdTree T, int Level, ItemType MinItem);
KdTree DeleteMax(KdTree T, int Level, ItemType MaxItem);
KdTree Insert(ItemType Item, KdTree T);
void PrintRange(ItemType Low, ItemType High, KdTree T);
void PrintItem(ItemType Item);

void test_kdtree(void);
void print_help(void);
void process_args(char *first);
void process_input(void);
void process_error(void);

struct KdNode {
    ItemType Data;
    KdTree   Left;
    KdTree   Right;
};

void Error(char *Message);

KdTree Initialize(void)
{
    return NULL;
}

static Position MP, M;
static UINT ML;

static void RecursiveFindMin(KdTree T, int TarLevel, int CurLevel, 
                             Position Prev)
{
    // if CurLevel == TarLevel then the left subtree of T must contain
    // the smallest child in terms of Tarlevel as long as left subtree
    // exists (if there is no left subtree then T itself represent the
    // smallest item); otherwise, T itself && both of its subtrees are
    // all probably contain the smallest item in terms of TarLevel
    if (CurLevel == TarLevel) {
        if (T->Left != NULL)
            return RecursiveFindMin(T->Left, TarLevel, !CurLevel, T);
    } else {
        if (T->Left != NULL)
            RecursiveFindMin(T->Left, TarLevel, !CurLevel, T);
        if (T->Right != NULL)
            RecursiveFindMin(T->Right, TarLevel, !CurLevel, T);
    }

    if (T->Data[TarLevel] <= M->Data[TarLevel]) {
        M = T;
        MP = Prev;
        ML = CurLevel;
    }
}

Position FindMin(KdTree T, int Level)
{
    Position Result;

    if (T == NULL)
        Error("Operate FindMin on emtpy tree");
    else {
        M = T;
        RecursiveFindMin(T, Level, 0, NULL);
        return Result = M;
    }
}

static KdTree HandleDeleteResult(KdTree T, ItemType MItem);

KdTree RecurDeleteMin(KdTree T, int TarLevel, int CurLevel, ItemType MinItem)
{
    M = T; MP = NULL; ML = 0;
    RecursiveFindMin(T, TarLevel, CurLevel, NULL);
    return HandleDeleteResult(T, MinItem);
}

KdTree DeleteMin(KdTree T, int Level, ItemType MinItem)
{
    if (T == NULL)
        Error("Operate DeleteMin on empty tree");
    else
        return RecurDeleteMin(T, Level, 0, MinItem);
}

static void RecursiveFindMax(KdTree T, int TarLevel, int CurLevel, 
                             Position Prev)
{
    if (CurLevel == TarLevel) {
        if (T->Right != NULL)
            return RecursiveFindMax(T->Right, TarLevel, !CurLevel, T);
    } else {
        if (T->Left != NULL)
            RecursiveFindMax(T->Left, TarLevel, !CurLevel, T);
        if (T->Right != NULL)
            RecursiveFindMax(T->Right, TarLevel, !CurLevel, T);
    }

    if (T->Data[TarLevel] >= M->Data[TarLevel]) {
        M = T;
        MP = Prev;
        ML = CurLevel;
    }
}

Position FindMax(KdTree T, int Level)
{
    Position Result;

    if (T == NULL)
        Error("Operate FindMin on emtpy tree");
    else {
        M = T;
        RecursiveFindMax(T, Level, 0, NULL);
        return Result = M;
    }
}

KdTree RecurDeleteMax(KdTree T, int TarLevel, int CurLevel, ItemType MinItem)
{
    M = T; MP = NULL; ML = 0;
    RecursiveFindMax(T, TarLevel, CurLevel, NULL);
    return HandleDeleteResult(T, MinItem);
}

KdTree DeleteMax(KdTree T, int Level, ItemType MinItem)
{
    if (T == NULL)
        Error("Operate DeleteMin on empty tree");
    else
        return RecurDeleteMax(T, Level, 0, MinItem);
}

KdTree HandleDeleteResult(KdTree T, ItemType MItem)
{
    MItem[0] = M->Data[0];
    MItem[1] = M->Data[1];

    if (M->Right != NULL)
        M->Right = RecurDeleteMin(M->Right, ML, !ML, M->Data);
    else if (M->Left != NULL)
        M->Left = RecurDeleteMax(M->Left, ML, !ML, M->Data);
    else {
        if (MP == NULL)
            T = NULL;
        else if (MP->Data[!ML] > M->Data[!ML])
            MP->Left = NULL;
        else
            MP->Right = NULL;
        free(M);
    }
    return T;
}

static KdTree RecursiveInsert(ItemType Item, KdTree T, int Level)
{
    if (T == NULL) {
        T = malloc(sizeof(struct KdNode));
        if (T == NULL)
            Error("Out of space");
        T->Left = T->Right = NULL;
        T->Data[0] = Item[0];
        T->Data[1] = Item[1];
    } else if (Item[Level] < T->Data[Level])
        T->Left = RecursiveInsert(Item, T->Left, !Level);
    else
        T->Right = RecursiveInsert(Item, T->Right, !Level);
    return T;
}

KdTree Insert(ItemType Item, KdTree T)
{
    return RecursiveInsert(Item, T, 0);
}

static void RecPrintRange(ItemType Low, ItemType High, KdTree T, int Level)
{
    if (T != NULL) {
        putchar('[');
        if (Low[0] <= T->Data[0] && T->Data[0] <= High[0] &&
            Low[1] <= T->Data[1] && T->Data[1] <= High[1]) {
            PrintItem(T->Data);
            putchar(' ');
        }
        if (Low[Level] <= T->Data[Level]) {
            printf("(l ");
            RecPrintRange(Low, High, T->Left, !Level);
            printf(")");
        }
        if (High[Level] >= T->Data[Level]) {
            printf("(r ");
            RecPrintRange(Low, High, T->Right, !Level);
            printf(")");
        }
        putchar(']');
    }
}

void PrintRange(ItemType Low, ItemType High, KdTree T)
{
    if (T == NULL)
        printf("(null)");
    else
        RecPrintRange(Low, High, T, 0);
    putchar('\n');
}

void PrintItem(ItemType Item)
{
    printf("(%d, %d)", Item[0], Item[1]);
}

void Error(char *Message)
{
    fprintf(stderr, "%s\n", Message);
    exit(1);
}

void test_kdtree(void)
{
    int i;
    KdTree T;
    ItemType Low, High, TmpCell, MinItem;

    Low[0] = Low[1] = 0;
    High[0] = High[1] = 10000;

    srand(time(NULL));

    T = Initialize();
    for (i = 0; i < 100; i++) {
        TmpCell[0] = rand() % 1000;
        TmpCell[1] = rand() % 1000;
        T = Insert(TmpCell, T);
    }

    PrintRange(Low, High, T);
    for (i = 0; i < 100; i++) {
        T = DeleteMin(T, i % 2, MinItem);
        printf("Deleted Item (Level %d): ", i % 2);
        PrintItem(MinItem);
        putchar('\n');
    }
    PrintRange(Low, High, T);
}

/* for user interface */
void print_help(void)
{  
    puts("\nAATree implementation.\n");
    puts(" KdTree [/h]");
    puts(" KdTree [/i]");
    puts(" KdTree [/t]\n");
    puts("  /h\tdisplay help info");
    puts("  /i\tto input points");
    puts("  /t\ttest KdTree implementation");
}

void process_args(char *first)
{
    if (strcmp(first, "/h") == 0 || strcmp(first, "/?") == 0)
        print_help();
    else if (strcmp(first, "/t") == 0)
        test_kdtree();
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