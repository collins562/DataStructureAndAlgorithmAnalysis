#ifndef _Huffman_H

typedef unsigned int UINT;
typedef unsigned char UCHAR;
typedef unsigned short USHORT;

#define OK       1  
#define ERROR   -1  
#define UNUSED  -1
#define USED    -2  
#define ARGS_ERR    -2          // 参数错误  
#define FILE_ERR    -3          // 文件错误  
#define HEAD_ERR    -4          // 头标错误  
#define MALLOC_ERR  -5          // 内存分配错误  
#define HUFTREE_ERR -6          // 霍夫曼树错误  
#define HUFCODE_ERR -7          // 霍夫曼编码错误  
#define CHAR_BITS   8           // 一个字符中的位数  
#define INT_BITS    32          // 一个整型中的位数  
#define CODE_SIZE   256         // 霍夫曼编码个数  
#define CACHE_SIZE  256         // I/O缓存大小  
#define UINT_SIZE   sizeof(UINT)  
#define UCHAR_SIZE  sizeof(UCHAR)  
#define USHORT_SIZE sizeof(USHORT)  
#define ZIP_HEAD    0xFFFFFFFF  // 压缩文件头标  
#define MAX_NAME_LEN    512  

struct TreeNode;
struct Code_struct;
typedef struct TreeNode htnode; // huffman tree node
typedef struct TreeNode *htnp;  // huffman tree node ptr
typedef struct Code_struct hufcode; // huffman code

htnp create_node(long w, short l, short r, short p);
int huftreecmp(htnp h1, htnp h2);
int create_huffmantree(long w[], int n, htnode *ht);
int encode_huffmantree(htnp hp, int n, hufcode *hc);
UCHAR chars_to_bits(const UCHAR chars[CHAR_BITS]);

int c_initial_files(char *src_filename, FILE **inp, char *obj_filename, FILE **outp);
long calc_data_frequency(FILE *in, long frequency[]);
int write_compress_file(FILE *in, FILE *out, htnp ht, hufcode *hc, char *src_filename, long src_filesize);
int compress(char *src_filename, char *obj_filename);
char *create_default_filename(char *src_filename, char *obj_filename);

int d_initial_files(char *src_filename, FILE **inp, char *obj_filename, FILE **outp);
void get_simple_huffmantree(FILE *in, short s_ht[][2]);
int write_decompress_file(FILE *in, FILE *out, short s_ht[][2], long bits_pos, long obj_filesize);
int find_last_sp_huffmantree(short s_ht[][2]);
int decompress(char *src_filename, char *obj_filename);

char *create_default_filename(char *src_filename, char *obj_filename);
void print_help(void);  
void process_error(int err_code);
void process_args(char *first, char *second, char *third);

#endif