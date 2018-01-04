#ifndef _BinPack_H

#define OK       1
#define ERROR   -1
#define ARGS_ERR    -2          // argument error
#define CAPA_ERR    -3          // capacity overflow error
#define MALLOC_ERR  -4          // memory allocation error
#define TESTNUM     100

struct staffnode;
struct binnode;
typedef struct staffnode *stnp; // staff node ptr
typedef struct binnode bnode; // bin node
typedef struct binstruct bset;

bset *create_bset(int maxbins);
void init_bset(bset *bsp);
void dispose_bset(bset *bsp);
void dispose_staffs(stnp sp);
void remove_staffs(bset *bsp);
stnp newnode(float size, stnp next);

int nextfit(float size, bset *bsp);
int firstfit(float size, bset *bsp);
int bestfit(float size, bset *bsp);
void print_bins(bset *bsp);

int test_fit(void);
int testfn(int (*fn)(float, bset*), int nt);
int fntime(int (*fn)(float, bset*), bset *bsp, int N, double *ttime, double *avbins);

void print_help();
void process_error(int err_code);
void process_args(char *first);

#endif