#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "BinPack.h"

#define Error(Message) fprintf(stderr, "%s\n", Message)

struct staffnode {
    float size;
    stnp  next;
};

struct binnode {
    float capacity;
    stnp firststaff;
};

struct binstruct {
    int   capacity;
    int   size;
    bnode *bins;
};

bset *create_bset(int maxbins)
{
    bset *bsp;

    if ((bsp = malloc(sizeof(struct binstruct))) != NULL) {
        if ((bsp->bins = malloc(sizeof(struct binnode) * maxbins)) == NULL)
            return NULL;
        bsp->capacity = maxbins;
        init_bset(bsp);
    }
    return bsp;
}

void init_bset(bset *bsp)
{
    int i;

    for (i = 0; i < bsp->capacity; i++) {
        bsp->bins[i].capacity = 1.0;
        bsp->bins[i].firststaff = NULL;
    }
    bsp->size = 1;  // bin set with 1 bin at first
}

void remove_staffs(bset *bsp)
{
    int i;

    for (i = 0; i < bsp->size; i++)
        dispose_staffs(bsp->bins[i].firststaff);
}

void dispose_bset(bset *bsp)
{
    remove_staffs(bsp);
    free(bsp);
}

void dispose_staffs(stnp sp)
{
    if (sp != NULL) {
        dispose_staffs(sp->next);
        free(sp);
    }
}

stnp newnode(float size, stnp next)
{
    stnp nst;

    if ((nst = (stnp) malloc(sizeof(struct staffnode))) != NULL) {
        nst->size = size;
        nst->next = next;
    }
    return nst;
}

int nextfit(float size, bset *bsp)
{
    bnode *bp;
    stnp nst;

    if (bsp->bins[bsp->size-1].capacity < size) {
        if (bsp->size == bsp->capacity)
            return CAPA_ERR;
        bp = &bsp->bins[bsp->size++];
    }
    else
        bp = &bsp->bins[bsp->size-1];

    if ((nst = newnode(size, bp->firststaff)) == NULL)
        return MALLOC_ERR;

    bp->capacity -= size;
    bp->firststaff = nst;
    return OK;
}

int firstfit(float size, bset *bsp)
{
    int i;
    bnode *bp;
    stnp nst;

    bp = NULL;
    for (i = 0; i < bsp->size; i++)
        if (bsp->bins[i].capacity >= size) {
            bp = &bsp->bins[i];
            break;
        }
    if (bp == NULL)
        if (bsp->size == bsp->capacity)
            return CAPA_ERR;
        else
            bp = &bsp->bins[bsp->size++];

    if ((nst = newnode(size, bp->firststaff)) == NULL)
        return MALLOC_ERR;

    bp->capacity -= size;
    bp->firststaff = nst;
    return OK;
}

int bestfit(float size, bset *bsp)
{
    int i;
    float c, min_differ;
    bnode *bp;
    stnp nst;

    min_differ = 1.0;
    bp = NULL;
    for (i = 0; i < bsp->size; i++)
        if ((c = bsp->bins[i].capacity) >= size && c - size < min_differ) {
            min_differ = c - size;
            bp = &bsp->bins[i];
        }
    if (bp == NULL)
        if (bsp->size == bsp->capacity)
            return CAPA_ERR;
        else
            bp = &bsp->bins[bsp->size++];

    if ((nst = newnode(size, bp->firststaff)) == NULL)
        return MALLOC_ERR;

    bp->capacity -= size;
    bp->firststaff = nst;
    return OK;
}

void print_bins(bset *bsp)
{
    int i;
    stnp sp;

    for (i = 0; i < bsp->size; i++) {
        printf("Bin %2d: ( ", i);
        for (sp = bsp->bins[i].firststaff; sp != NULL; sp = sp->next)
            printf("%.1f ", sp->size);
        puts(")\n");
    }
    putchar('\n');
}

int test_fit(void)
{
    int i, j, tsize, hsize, err_code;
    int ts[] = { 100, 1000, 10000 };
    char *headings[] = { "nextfit ", "firstfit", "bestfit " };
    int (*fns[])(float, bset*) = { nextfit, firstfit, bestfit };

    tsize = sizeof(ts) / sizeof(ts[0]);
    hsize = sizeof(headings) / sizeof(headings[0]); // equals with fn nums

    printf("         |");
    for (i = 0; i < hsize; i++)
        printf("       %s       |", headings[i]);
    putchar('\n');
    for (i = 0; i < tsize; i++) {
        printf(" %7d |", ts[i]);
        for (j = 0; j < hsize; j++)
            if ((err_code = testfn(fns[j], ts[i])) != OK)
                return err_code;
        putchar('\n');
    }
    return OK;
}

int testfn(int (*fn)(float, bset*), int nt)
{
    int err_code;
    double t, avbins;
    bset *bsp;

    bsp = create_bset(nt);
    if (bsp == NULL)
        return MALLOC_ERR;
    
    avbins = 0.0;
    if ((err_code = fntime(fn, bsp, nt, &t, &avbins)) != OK)
        return err_code;
    dispose_bset(bsp);
    printf(" %.5fs (%4.f bins) |", t, avbins);
    return OK;
}

int fntime(int (*fn)(float, bset*), bset *bsp, int N, double *ttime, double *avbins)
{
    int err_code;
    int start, end, tclocks = 0;
    int i, j;
    double rsizes[N];

    for (i = 0; i < TESTNUM; i++) {
        srand(time(NULL));
        for (j = 0; j < N; j++)
            rsizes[j] = (float) (rand() % 82 + 9) / 100; // 0.09 ~ 0.9
        remove_staffs(bsp);
        init_bset(bsp);

        start = clock();
        for (j = 0; j < N; j++)
            if ((err_code = fn(rsizes[j], bsp)) != OK)
                return err_code;
        end = clock();
        tclocks += end - start;
        *avbins += bsp->size;
    }
    *avbins /= TESTNUM;
    *ttime = (double) tclocks / CLOCKS_PER_SEC / TESTNUM;
    return OK;
}

/* for user interface */
void print_help()
{  
    puts("\nBin packing problem.\n");
    puts("BinPack [/h]");
    puts("BinPack [/t]\n");
    puts("  /h\tdisplay help info");
    puts("  /t\ttest all fit algorithm");
}

void process_error(int err_code)
{
    switch (err_code) {
        case CAPA_ERR   : Error("\n\tbin set capacity overflow."); break;
        case MALLOC_ERR : Error("\n\tFailed to allocate memory."); break;
        case ARGS_ERR   : Error("\n\tInvalid arguments."); break;
        default         : break;
    }
}

void process_args(char *first)
{
    if (strcmp(first, "/h") == 0 || strcmp(first, "/?") == 0)
        print_help();
    else if (strcmp(first, "/t") == 0)
        process_error(test_fit());
    else
        process_error(ARGS_ERR);
}

int main(int argc, char *argv[])
{
    switch (argc) {
        case   1: print_help(); break;
        case   2: process_args(argv[1]); break;
        default : process_error(ARGS_ERR);
    }
    return OK;
}