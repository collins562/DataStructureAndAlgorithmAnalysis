#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define OK      1
#define infinity 100000
#define Error(Message) fprintf(stderr, "%s\n", Message)

typedef int tdimarray[10][10];

void optmatrix(const long c[], int n, tdimarray m, tdimarray lastchange);
void print_tdimarray(tdimarray A, int N);
void test_matrix(void);

void print_help(void);
void process_args(char *first);
void process_error(void);

void optmatrix(const long c[], int n, tdimarray m, tdimarray lastchange)
{
    int i, k, left, right;
    long thism;

    print_tdimarray(m, n);
    print_tdimarray(lastchange, n);
    for (left = 1; left <= n; left++)
        m[left][left] = 0;
    for (k = 1; k < n; k++)
        for (left = 1; left <= n - k; left++) {
            right = left + k;
            m[left][right] = infinity;
            for (i = left; i < right; i++) {
                thism = m[left][i] + m[i+1][right] + c[left-1] * c[i] * c[right];
                if (thism < m[left][right]) {
                    m[left][right] = thism;
                    lastchange[left][right] = i;
                }
            }
        }
}

void print_tdimarray(tdimarray A, int N)
{
    int i, k;

    printf("   ");
    for (i = 1; i <= N; i++)
        printf("   %02d", i);
    putchar('\n');
    for (i = 1; i <= N; i++) {
        printf(" %02d", i);
        for (k = 1; k <= N; k++)
            if (A[i][k] < 0)
                printf(" ----");
            else
                printf(" %04d", A[i][k]);
        putchar('\n');
    }
}

void test_matrix(void)
{
    int i, j, tsize, hsize, err_code;
    long ts[] = { 20, 10, 20, 1, 40, 5, 30};
    long ts1[] = { 10, 50, 10, 40, 30 };
    tdimarray m, change;

    tsize = sizeof(ts1) / sizeof(ts1[0]) - 1;

    for (i = 0; i <= tsize; i++)
        for (j = 0; j <= tsize; j++)
            m[i][j] = change[i][j] = -1;
    optmatrix(ts1, tsize, m, change);
    print_tdimarray(m, tsize);
    print_tdimarray(change, tsize);
}

/* for user interface */
void print_help(void)
{  
    puts("\nOptimal matrix problem.\n");
    puts(" optmatrix [/h]");
    puts(" optmatrix [/t]\n");
    puts("  /h\tdisplay help info");
    puts("  /t\ttest optimal optmatrix algorithm");
}

void process_args(char *first)
{
    if (strcmp(first, "/h") == 0 || strcmp(first, "/?") == 0)
        print_help();
    else if (strcmp(first, "/t") == 0)
        test_matrix();
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
