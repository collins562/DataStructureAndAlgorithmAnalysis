#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define OK          1
#define NotAVertex  -1
#define Infinity    100000
#define Error(Message) fprintf(stderr, "%s\n", Message)

typedef int graph[10][10];

void init_graph(graph G);
void allpairs(graph A, graph D, graph path, int n);
void print_graph(graph A, int N);
void printpath( graph Path, int S, int T);
void test_dijkstra(void);

void print_help(void);
void process_args(char *first);
void process_error(void);

void init_graph(graph G)
{
    int i, j;

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++)
            G[i][j] = Infinity;
        G[i][i] = 0;
    }
}

void allpairs(graph A, graph D, graph path, int n)
{
    int i, j, k;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            D[i][j] = A[i][j];
            path[i][j] = NotAVertex;
        }

    for (k = 0; k < n; k++) {
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                if (D[i][k] + D[k][j] < D[i][j]) {
                    D[i][j] = D[i][k] + D[k][j];
                    path[i][k] = k;
                    // in array path[i], there may be several path index (0~k);
                    // but apparently the biggest
                }
        printf("\nChecking intermediate vertex: %02d\n\n", k);
        print_graph(A, n);
        putchar('\n');
        print_graph(D, n);
        putchar('\n');
        print_graph(path, n);
    }
}

void printpath1( graph Path, int S, int T )
{
    int StopOver = Path[S][T];
    printf("------------------\n");
    if( S != T && StopOver != NotAVertex )
    {
        printpath1(Path, S, StopOver);
        printf("%d to ", StopOver);
        printpath1(Path, StopOver, T);
    }
}
/* Assume the existence of a Path of length at least 1 */
void printpath( graph Path, int S, int T)
{
    printf("%d to ", S);
    printpath1(Path, S, T);
    printf("%d\n", T);
}

void print_graph(graph A, int N)
{
    int i, k;

    printf("   ");
    for (i = 0; i < N; i++)
        printf("  %02d ", i);
    putchar('\n');
    for (i = 0; i < N; i++) {
        printf(" %02d", i);
        for (k = 0; k < N; k++)
            if (A[i][k] == Infinity)
                printf(" ----");
            else
                printf(" [%02d]", A[i][k]);
        putchar('\n');
    }
}

void test_dijkstra(void)
{
    int i, j;
    graph A, D, path;

    init_graph(A);

    A[0][1] = 3; // s->A
    A[0][2] = 2; // s->B
    A[1][2] = 1; // A->B
    A[1][3] = 3; // A->C
    A[1][4] = 4; // A->D
    A[2][4] = 2; // B->D
    A[3][5] = 2; // C->t
    A[4][5] = 3; // D->t

    allpairs(A, D, path, 6);
    print_graph(A, 6);
    print_graph(D, 6);
    print_graph(path, 6);
    printpath(path, 0, 4);
}

/* for user interface */
void print_help(void)
{  
    puts("\nDijkstra algorithm.\n");
    puts(" dijkstra [/h]");
    puts(" dijkstra [/t]\n");
    puts("  /h\tdisplay help info");
    puts("  /t\ttest dijkstra algorithm");
}

void process_args(char *first)
{
    if (strcmp(first, "/h") == 0 || strcmp(first, "/?") == 0)
        print_help();
    else if (strcmp(first, "/t") == 0)
        test_dijkstra();
    else
        process_error();
}

void process_error(void)
{
    Error("Arguments Error\n");
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
