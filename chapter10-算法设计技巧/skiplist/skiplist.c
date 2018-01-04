#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define OK       1
#define MaxOrder 8

struct listnode;
struct header;
typedef struct listnode *node;
typedef struct header *slist;

slist create_skiplist(void);
int isfull(slist L);
int isempty(slist L);
void dispose_slist(slist L);
node create_node(int elt, int order);
node slist_find(int elt, slist L);
void slist_insert(int elt, slist L);
void slist_expand(slist L);
void slist_delete(int elt, slist L);
void print_slist(slist L);

void test_skiplist(void);
void print_help();
void process_args(char *first);
void process_input(void);
void process_error(void);

struct listnode {
    int elt;
    int order;
    node *ns;  // node list
};

struct header {
    int size;
    int order;
    int ordersize[MaxOrder]; // keep track of size of each order
    node *ns;                // node list
};

slist create_skiplist(void)
{
    int i;
    slist L;

    // take 4 as default order
    L = (slist) malloc(sizeof(struct header));
    if (L != NULL) {
        L->ns = (node*) malloc(sizeof(node) * 1);
        if (L->ns == NULL)
            return NULL;
        L->ns[0] = NULL;
        L->size = 0;
        L->order = 1;
        for (i = 0; i < MaxOrder; i++)
            L->ordersize[i] = 0;
    }
}

int isfull(slist L)
{
    return L->size == (1 << L->order) - 1;
}

int isempty(slist L)
{
    return L->size == 0;
}

void dispose_slist(slist L)
{
    int i;
    node n, tmp;

    for (n = L->ns[0]; n != NULL; n = tmp) {
        tmp = n->ns[0];
        free(n);
    }
    free(L);
}

node create_node(int elt, int order)
{
    int i;
    node n;

    n = (node) malloc(sizeof(struct listnode));
    if (n != NULL) {
        n->ns = (node*) malloc(sizeof(node) * order);
        if (n->ns == NULL)
            return NULL;
        n->elt = elt;
        n->order = order;
        for (i = 0; i < order; i++)
            n->ns[i] = NULL;
    }

    return n;
}

node slist_find_1(int elt, slist L, node *lastn)
{
    int cur_order;
    node n, *nlist;

    nlist = L->ns; *lastn = NULL;
    for (cur_order = L->order; cur_order > 0; cur_order--) {
        for (n = nlist[cur_order - 1]; n != NULL; n = n->ns[cur_order - 1]) {
            if (n->elt == elt)
                return n;
            else if (n->elt > elt)
                break;
            *lastn = n;
        }
        if (*lastn != NULL)
            nlist = (*lastn)->ns;
    }

    return NULL;
}

node slist_find(int elt, slist L)
{
    node ignored;

    return slist_find_1(elt, L, &ignored);
}

void slist_insert(int elt, slist L)
{
    int i, j, rn, ro; // random num; random order
    node n, lastn, newn;
    node *last_nlist;

    if (slist_find_1(elt, L, &lastn) != NULL)
        return; // elt already exists; do nothing

    if (isfull(L))
        slist_expand(L);
    // pick random order in rest space
    rn = rand() % ((1 << L->order) - 1 - L->size) + 1;
    for (ro = 0; rn > 0; ro++)
        // rn minus rest space in each order
        rn -= (1 << (L->order - ro - 1)) - L->ordersize[ro];

    if ((newn = create_node(elt, ro)) == NULL) {
        fprintf(stderr, "%s\n", "Failed to insert.");
        return;
    }

    // copy node list from last node
    i = 0;
    if (lastn != NULL)  // lastn == NULL means L is empty
        for ( ; i < lastn->order && i < ro; i++) {
            newn->ns[i] = lastn->ns[i];
            lastn->ns[i] = newn;
        }
    for ( ; i < ro; i++) {
        last_nlist = L->ns;
        for (n = last_nlist[i]; n != NULL; n = n->ns[i]) {
            if (n->elt > elt)
                break;
            last_nlist = n->ns;
        }
        newn->ns[i] = last_nlist[i];
        last_nlist[i] = newn;
    }

    L->size++;
    L->ordersize[ro - 1]++;
}

void slist_expand(slist L)
{
    int i;
    node *new_nlist;

    new_nlist = (node*) malloc(sizeof(node) * (L->order + 1));
    if (new_nlist == NULL) {
        fprintf(stderr, "%s\n", "Failed to expand skiplist.");
        exit(1);
    }

    for (i = 0; i < L->order; i++)
        new_nlist[i] = L->ns[i];
    new_nlist[i] = NULL;
    
    free(L->ns);
    L->ns = new_nlist;
    L->order++;
}

void slist_delete(int elt, slist L)
{
    int i;
    node n, dn, lastn; // node to be deleted; last node before dn;
    node *last_nlist;

    if ((dn = slist_find_1(elt, L, &lastn)) == NULL)
        return; // elt not found; do nothing

    i = 0;
    if (lastn != NULL)
        for ( ; i < lastn->order && i < dn->order; i++)
            lastn->ns[i] = dn->ns[i];

    // if lastn->order < dn->order, extra work should be done
    for ( ; i < dn->order; i++) {
        last_nlist = L->ns;
        for (n = last_nlist[i]; n != NULL; n = n->ns[i]) {
            if (n == dn)
                break;
            last_nlist = n->ns;
        }
        last_nlist[i] = dn->ns[i];
    }
    L->size--;
    L->ordersize[dn->order - 1]--;
    free(dn);
}

void print_slist(slist L)
{
    int i;
    node n;

    printf("------\n");
    for (n = L->ns[0]; n != NULL; n = n->ns[0]) {
        printf("%3d ", n->elt);
        for (i = 0; i < n->order; i++)
            putchar('*');
        putchar('\n');
    }
    printf("------\n");
}

void test_skiplist(void)
{
    int i;
    slist L;

    srand(time(NULL));
    L = create_skiplist();
    for (i = 0; i < 200; i++)
        slist_insert(rand() % 400, L);

    print_slist(L);
    for (i = 0; i < 100; i++)
        if (slist_find(i, L) != NULL) {
            printf("Delete %d...\n", i);
            slist_delete(i, L);
        }
    print_slist(L);
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