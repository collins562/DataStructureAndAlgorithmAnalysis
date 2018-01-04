#ifndef _BinPack_H

#define OK       1
#define ERROR   -1
#define ARGS_ERR    -2          // argument error
#define CAPA_ERR    -3          // capacity overflow error
#define MALLOC_ERR  -4          // memory allocation error

#define Infinity    100000
#define TESTNUM     1000

struct point;
typedef struct point pt;
typedef struct point *ptp;
struct pointpair;
typedef struct pointpair ptpair;

double point_dist(ptp p1, ptp p2);
void divide_points_by_x(ptp *psorty, int np, int x, ptp *lpsorty, ptp *rpsorty);
int find_strip_points(ptp *psorty, int np, int x, double offset, ptp *strip);
void find_nearest(ptp *psortx, int start, int end, ptp *psorty, ptpair *pair);
void nearest(ptp pts, int np, ptpair *pair);
int x_coordinate_cmp(const void *p1, const void *p2);
int y_coordinate_cmp(const void *p1, const void *p2);
void print_point(ptp p);
void print_pair(ptpair *pair);

void test_nearest(void);
double fntime(int N);

void print_help();
void process_args(char *first);
void process_input(void);
void process_error(void);

#endif