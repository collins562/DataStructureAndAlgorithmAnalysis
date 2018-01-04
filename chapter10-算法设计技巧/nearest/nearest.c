#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "nearest.h"

#define Error(Message) fprintf(stderr, "%s\n", Message)

struct point {
    int x;
    int y;
};

struct pointpair {
    ptp    p1;
    ptp    p2;
    double dist;
};

double point_dist(ptp p1, ptp p2)
{
    return sqrt(pow(p1->x - p2->x, 2) + pow(p1->y - p2->y, 2));
}

void divide_points_by_x(ptp *psorty, int np, int x, ptp *lpsorty, ptp *rpsorty)
{
    int i, j, k, left_np = (np + 1) / 2, right_np = np - left_np;

    for (i = j = k = 0; i < np; i++)
        if (psorty[i]->x > x) {
            rpsorty[j++] = psorty[i];
            if (j != right_np)      // in case that there are points with same x
                continue;
            while (k < left_np)
                lpsorty[k++] = psorty[i++];
        } else {
            lpsorty[k++] = psorty[i];
            if (k != left_np)
                continue;
            while (j < right_np)
                rpsorty[j++] = psorty[i++];
        }
}

int find_strip_points(ptp *psorty, int np, int x, double offset, ptp *strip)
{
    int i, snp;
    double lower = x - offset, upper = x + offset;

    for (i = snp = 0; i < np; i++)
        if (psorty[i]->x > lower && psorty[i]->x < upper)
            strip[snp++] = psorty[i];
    return snp;
}

void find_nearest(ptp *psortx, int start, int end, ptp *psorty, ptpair *pair)
{
    int i, j, mid, snp, np = end - start + 1;
    double cur_dist;
    ptp lpsorty[np], rpsorty[np], strip[np];

    if (np == 2) {
        if (pair->dist > (cur_dist = point_dist(psortx[start], psortx[end]))) {
            pair->p1 = psortx[start];
            pair->p2 = psortx[end];
            pair->dist = cur_dist;
        }
    } else if (np > 2) {
        mid = (start + end) / 2;

        divide_points_by_x(psorty, np, psortx[mid]->x, lpsorty, rpsorty);
        find_nearest(psortx, start, mid, lpsorty, pair);
        find_nearest(psortx, mid+1, end, rpsorty, pair);

        snp = find_strip_points(psorty, np, psortx[mid]->x, pair->dist, strip);
        for (i = 0; i < snp - 1; i++)
            for (j = i + 1; j < snp; j++)
                if (psorty[j]->y - psorty[i]->y > pair->dist)
                    break;
                else if ((cur_dist = point_dist(psorty[j], psorty[i])) < pair->dist) {
                    pair->p1 = psorty[i];
                    pair->p2 = psorty[j];
                    pair->dist = cur_dist;
                }
    }
    // do nothing when np <= 1
}


void nearest(ptp pts, int np, ptpair *pair)
{
    int i;
    ptp psortx[np], psorty[np];

    for (i = 0; i < np; i++)
        psortx[i] = psorty[i] = &pts[i];
    qsort(psortx, np, sizeof(ptp), x_coordinate_cmp);
    qsort(psorty, np, sizeof(ptp), y_coordinate_cmp);

    // pair->p1 = pair->p2 = NULL;
    pair->dist = Infinity;

    find_nearest(psortx, 0, np-1, psorty, pair);
}

int x_coordinate_cmp(const void *p1, const void *p2)
{
    return (*((ptp*) p1))->x - (*((ptp*) p2))->x;
}

int y_coordinate_cmp(const void *p1, const void *p2)
{
    return (*((ptp*) p1))->y - (*((ptp*) p2))->y;
}

void print_point(ptp p)
{
    printf("(%d, %d)", p->x, p->y);
}

void print_pair(ptpair *pair)
{
    putchar('[');
    print_point(pair->p1);
    putchar(' ');
    print_point(pair->p2);
    puts("]");
}

void test_nearest(void)
{
    int i, j, tsize, hsize, err_code;
    int ts[] = { 60, 600, 6000, 60000 };

    tsize = sizeof(ts) / sizeof(ts[0]);

    printf("         |");
    printf("  nearest   |\n");
    for (i = 0; i < tsize; i++)
        printf(" %7d | %.6f s |\n", ts[i], fntime(ts[i]));
}

unsigned long ulrand(void) 
{ 
    return ((((unsigned long) rand()<<24) & 0xFF000000ul) |
            (((unsigned long) rand()<<12) & 0x00FFF000ul) |
            (((unsigned long) rand()    ) & 0x00000FFFul));
}

double fntime(int N)
{
    int err_code;
    int start, end, tclocks = 0;
    int i, j, k;
    ptp pts;
    ptpair pair;

    if ((pts = (ptp) malloc(sizeof(struct point) * N)) == NULL) {
        fprintf(stderr, "%s\n", "\nFailed to allocate memory");
        exit(1);
    }

    srand(time(NULL));
    for (i = 0; i < TESTNUM; i++) {
        for (j = 0; j < N; j++) {
            pts[j].x = ulrand() % N;
            pts[j].y = ulrand() % N;
        }

        start = clock();
        nearest(pts, N, &pair);
        end = clock();
        tclocks += end - start;
    }
    return (double) tclocks / CLOCKS_PER_SEC / TESTNUM;
}

/* for user interface */
void print_help()
{  
    puts("\nNearest points problem.\n");
    puts(" Nearest [/h]");
    puts(" Nearest [/i]");
    puts(" Nearest [/t]\n");
    puts("  /h\tdisplay help info");
    puts("  /i\tto input points");
    puts("  /t\ttest nearest points algorithm");
}

void process_args(char *first)
{
    if (strcmp(first, "/h") == 0 || strcmp(first, "/?") == 0)
        print_help();
    else if (strcmp(first, "/t") == 0)
        test_nearest();
    else if (strcmp(first, "/i") == 0)
        process_input();
    else
        process_error();
}

void process_input(void)
{
    int i, np;
    ptp pts;
    ptpair pair;

    printf("Please input points numbers. End with newline: ");
    scanf("%d", &np);

    pts = malloc(sizeof(struct point) * np); 
    printf("\nPlease input point coordinate (pattern: 'x y'). End with newline:\n");
    for (i = 0; i < np; i++)
        scanf("%d %d", &pts[i].x, &pts[i].y);
    nearest(pts, np, &pair);
    printf("Nearest points:\n");
    printf("\t(%d, %d)\n", pair.p1->x, pair.p1->y);
    printf("\t(%d, %d)\n", pair.p2->x, pair.p2->y);
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