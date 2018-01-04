#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "Avltree.h"

#define OK          1
#define Error       -1
#define RECON_ERROR -2
#define ARGS_ERROR  -3

#define TRUE        1
#define FALSE       0

typedef AvlTree distset;

int reconstruct(int d[], int n);
int turnpike(int X[], distset D, int n);
int place(int X[], distset D, int n, int left, int right);

void test_skiplist(void);
void print_help();
void process_args(char *first);
int process_input(void);
void process_error(int err_code);

int reconstruct(int d[], int n)
{
    int i, dn, *X;
    distset D = NULL;

    for (dn = sqrt(n); dn * (dn - 1) / 2 < n ; dn++)
        ;
    if (dn * (dn - 1) / 2 != n)
        return RECON_ERROR;

    X = (int*) malloc(sizeof(int) * (dn + 1));

    for (i = 0; i < n; i++)
        Insert(d[i], &D);

    if (turnpike(X, D, dn) == TRUE) {
        printf("Turnpike found: \n\t[ ");
        for (i = 1; i < dn+1; i++)
            printf("%d ", X[i]);
        printf("]\n");
    } else {
        printf("Turnpike not found.\n");
    }

    return OK;
}

int turnpike(int X[], distset D, int n)
{
    X[1] = 0;
    X[n] = DeleteMax(&D);
    X[n-1] = DeleteMax(&D);
    printf("n: %d; X[n] = %d; X[1] = %d; X[n-1] = %d\n", n, X[n], X[1], X[n-1]);
    if (Find(X[n] - X[n-1], D) != NULL) {
        Delete(X[n] - X[n-1], &D);
        return place(X, D, n, 2, n - 2);
    } else
        return FALSE;
}

int place(int X[], distset D, int n, int left, int right)
{
    int i, Dmax, found = FALSE;

    if (IsEmpty(D))
        return TRUE;
    Dmax = Retrieve(FindMax(D));

    // check if setting X[right] = Dmax is feasible
    for (i = 1; i <= n; i++) {
        if (i == left)
            i = right + 1;
        if (Find(fabs(X[i] - Dmax), D) == NULL)
            break;
    }

    if (i == n + 1) {  // feasible
        X[right] = Dmax;
        for (i = 1; i <= n; i++) {
            if (i == left)
                i = right + 1;
            Delete(fabs(X[i] - Dmax), &D);
        }
        found = place(X, D, n, left, right - 1);
        if (!found)
            for (i = 1; i <= n; i++) {
                if (i == left)
                    i = right + 1;
                Insert(fabs(X[i] - Dmax), &D);
            }
    }

    // if first attempt failed, try to see if setting 
    // X[left] = X[n] - Dmax is feasible
    if (!found) {
        for (i = 1; i <= n; i++) {
            if (i == left)
                i = right + 1;
            if (Find(fabs(X[n] - Dmax - X[i]), D) == NULL)
                break;
        }

        if (i == n + 1) { // feasible
            X[left] = X[n] - Dmax;
            for (i = 1; i <= n; i++) {
                if (i == left)
                    i = right + 1;
                Delete(fabs(X[n] - Dmax - X[i]), &D);
            }
            found = place(X, D, n, left + 1, right);

            if (!found)
                for (i = 1; i <= n; i++)
                    if (i == left)
                        i = right + 1;
                    Insert(fabs(X[n] - Dmax - X[i]), &D);
        }
    }

    return found;
}

void test_turnpike(void)
{
    int *Xp;
    distset D = NULL;

    int ts[] = { 1, 2, 2, 2, 3, 3, 3, 4, 5, 5, 5, 6, 7, 8, 10 };

    reconstruct(ts, 15);
}

/* for user interface */
void print_help()
{  
    puts("\nturnpike reconstruction problem.\n");
    puts(" turnpike [/h]");
    puts(" turnpike [/i]");
    puts(" turnpike [/t]\n");
    puts("  /h\tdisplay help info");
    puts("  /i\tto input distances");
    puts("  /t\ttest turnpike implementation");
}

void process_args(char *first)
{
    if (strcmp(first, "/h") == 0 || strcmp(first, "/?") == 0)
        print_help();
    else if (strcmp(first, "/t") == 0)
        test_turnpike();
    else if (strcmp(first, "/i") == 0)
        process_error(process_input());
    else
        process_error(ARGS_ERROR);
}

int process_input(void)
{

}

void process_error(int err_code)
{
    switch (err_code) {
        case ARGS_ERROR  : fprintf(stderr, "Arguments Error\n"); print_help(); break;
        case RECON_ERROR : fprintf(stderr, "Reconstruction Error\n"); break;
    }
}

int main(int argc, char *argv[])
{
    switch (argc) {
        case   1: print_help(); break;
        case   2: process_args(argv[1]); break;
        default : process_error(ARGS_ERROR);
    }
    return OK;
}