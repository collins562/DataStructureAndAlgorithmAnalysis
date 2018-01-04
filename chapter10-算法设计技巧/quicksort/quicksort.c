
/* compare different median choose method */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define OK      1
#define cutoff  3
#define TESTNUM 100
#define Error(Message) fprintf(stderr, "%s\n", Message)

void insertionsort(int A[], int N);

int median3(int A[], int left, int right);
void qsort1(int A[], int left, int right);
void quicksort1(int A[], int N);

int median_of_median_of_5(int A[], int left, int right, int itv);
void qsort2(int A[], int left, int right, int itv);
void quicksort2(int A[], int N);

void print_array(int A[], int offset, int N);

int test_qsort(void);
void testfn(void (*fn)(int [], int), int nt);
double fntime(void (*fn)(int [], int), int N);
unsigned long ulrand(void);
void print_help();
void process_args(char *first);
void process_error(void);

static void swap(const void *p1, const void *p2)
{
    int tmp = *((int *) p1);
    *((int *) p1) = *((int *) p2);
    *((int *) p2) = tmp;
}

/* Insertion Sort */
static void insertionsort_interval(int A[], int N, int itv)
{
    int i, j, tmp;

    for (i = itv; i < N; i += itv) {
        tmp = A[i];
        for (j = i; j > 0 && A[j - itv] > tmp; j -= itv)
            A[j] = A[j - itv];
        A[j] = tmp;
    }
}

void insertionsort(int A[], int N)
{
    insertionsort_interval(A, N, 1);
}

/////////////////////////////////
/* quick sort with median of 3 */
/////////////////////////////////

int median3(int A[], int left, int right)
{
    int center = (left + right) / 2;

    if (A[left] > A[center])
        swap(&A[left], &A[center]);
    if (A[left] > A[right])
        swap(&A[left], &A[right]);
    if (A[center] > A[right])
        swap(&A[center], &A[right]);

    /* Invariant: A[left] <= A[center] <= A[right]   */
    swap(&A[center], &A[right - 1]); /* Hide pivot   */
    return A[right - 1];             /* Return pivot */
}

void qsort1(int A[], int left, int right)
{
    int i, j;
    int pivot;

    if (left + cutoff <= right) {
        pivot = median3(A, left, right);
        i = left; j = right - 1;
        for ( ; ; ) {
            while (A[++i] < pivot) {}
            while (A[--j] > pivot) {}
            if (i < j)
                swap(&A[i], &A[j]);
            else
                break;
        }
        swap(&A[i], &A[right - 1]); /* Restore pivot */

        qsort1(A, left, i - 1);
        qsort1(A, i + 1, right);
    } else /* Do an insertion sort on the subarray   */
        insertionsort(A + left, right - left + 1);
}

void quicksort1(int A[], int N)
{
    qsort1(A, 0, N - 1);
}

///////////////////////////////////////////
/* quick sort with median of median of 5 */
///////////////////////////////////////////

int median_of_median_of_5(int A[], int left, int right, int itv)
{
    int i, j, median;
    int next_itv = itv * 5;
    int tmpn = (right - left + 1) / itv / 5;

    j = left;
    for (i = 0, j = left; i < tmpn; i++, j += next_itv)
        insertionsort_interval(A + j, 5, itv);

    qsort2(A, left + next_itv / 2, right, next_itv);

    median = left + next_itv / 2 + tmpn / 2 * next_itv;
    swap(&A[median], &A[right]);
    return A[right];
}

void qsort2(int A[], int left, int right, int itv)
{
    int i, j, end, offset, pivot, prev_itv = itv / 5;
    
    offset = itv / 2;
    end = right - (right - left + offset + 1) % itv - offset;

    if (left + cutoff * itv < end) {
        pivot = median_of_median_of_5(A, left, right, itv);
        i = left; j = right;
        for ( ; ; ) {
            while ((i += itv) < right + 1 && A[i] < pivot) {}
            if (j > end)
                while (A[j -= prev_itv] > pivot) {}
            else
                while ((j -= itv) > left && A[j] > pivot) {}
            if (i < j && j > left)
                swap(&A[i], &A[j]);
            else
                break;
        }
        if (i > right)
            i -= itv;
        swap(&A[i], &A[right]); /* Restore pivot */

        qsort2(A, left, i - 1, itv);
        qsort2(A, i + 1, right, itv);
    } else /* Do an insertion sort on the subarray   */
        insertionsort_interval(A + left, right - left + 1, itv);
}

void quicksort2(int A[], int N)
{
    qsort2(A, 0, N - 1, 1);
}


void print_array(int A[], int offset, int N)
{
    int i;

    printf("[ ");
    for (i = offset; i < N; i++)
        printf("%02d ", A[i]);
    printf("]\n");
}

int test_qsort(void)
{
    int i, j, tsize, hsize, err_code;
    int ts[] = { 1000, 10000, 100000 };
    char *headings[] = { " qsort_m3 ", "qsort_mom5" };
    void (*fns[])(int [], int) = { quicksort1, quicksort2 };

    tsize = sizeof(ts) / sizeof(ts[0]);
    hsize = sizeof(headings) / sizeof(headings[0]); // equals with fn nums

    printf("         |");
    for (i = 0; i < hsize; i++)
        printf(" %s |", headings[i]);
    putchar('\n');
    for (i = 0; i < tsize; i++) {
        printf(" %7d |", ts[i]);
        for (j = 0; j < hsize; j++)
            testfn(fns[j], ts[i]);
        putchar('\n');
    }
    return OK;
}

void testfn(void (*fn)(int [], int), int nt)
{
    printf(" %.6f s |", fntime(fn, nt));
}

double fntime(void (*fn)(int [], int), int N)
{
    int start, end, tclocks = 0;
    int i, j, A[N];

    srand(time(NULL));
    for (i = 0; i < TESTNUM; i++) {
        for (j = 0; j < N; j++)
            A[j] = ulrand() / N; // 0.09 ~ 0.9

        start = clock();
        fn(A, N);
        end = clock();
        tclocks += end - start;
    }
    return (double) tclocks / CLOCKS_PER_SEC / TESTNUM;
}

unsigned long ulrand(void) 
{ 
    return ((((unsigned long) rand()<<24) & 0xFF000000ul) |
            (((unsigned long) rand()<<12) & 0x00FFF000ul) |
            (((unsigned long) rand()    ) & 0x00000FFFul));
}

/* for user interface */
void print_help()
{  
    puts("\nQuicksort problem.\n");
    puts(" quicksort [/h]");
    puts(" quicksort [/t]\n");
    puts("  /h\tdisplay help info");
    puts("  /t\ttest quick sort points algorithm");
}

void process_args(char *first)
{
    if (strcmp(first, "/h") == 0 || strcmp(first, "/?") == 0)
        print_help();
    else if (strcmp(first, "/t") == 0)
        test_qsort();
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
