#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Huffman.h"
#include "BinHeap.h" 

#define Error(Message) fprintf(stderr, "%s\n", Message)

struct TreeNode {
    // TreeNode to encode character
    long weight;
    short index, p, l, r; // parent; left; right
};

struct Code_struct {
    UCHAR len;
    UCHAR *codestr;
};

/////////////////////////////////
/* Huffman Tree implementation */
/////////////////////////////////

int huftreecmp(htnp h1, htnp h2)
{
    return h1->weight - h2->weight;
}

htnp create_node(long w, short l, short r, short p)
{
    htnp nh;

    if ((nh = (htnode*) malloc(sizeof(struct TreeNode))) != NULL) {
        nh->weight = w;
        nh->index  = UNUSED;
        nh->l = l;
        nh->r = r;
        nh->p = p;
    }
    return nh;
}

/* create huffman tree according to given weights w             */
/* return OK if there is no error, otherwise return HUFTREE_ERR */
int create_huffmantree(long w[], int n, htnode *ht)
{
    int  i, m;
    int  s1, s2;
    htnp h1, h2, h3;
    PriorityQueue H;

    if (n < 1)
        return HUFTREE_ERR;

    H = InitBinHeap(CODE_SIZE);
    for (i = 0; i < n; i++) {
        ht[i].index = i;
        if ((ht[i].weight = w[i]) == 0)
            continue;
        ht[i].p = ht[i].l = ht[i].r = UNUSED;
        RawInsert(&ht[i], H);
    }
    BuildHeap(H);   // O(N)
    for ( ; i < n * 2 - 1; i++) {
        ht[i].index = i;
        ht[i].weight = 0;
        ht[i].p = ht[i].l = ht[i].r = UNUSED;
    }

    i = n;
    while (H->Size > 1) { // O(N logN)
        // there are H->Size-1 merges
        h1 = DeleteMin(H); h2 = DeleteMin(H);
        h3 = &ht[i];
        h3->weight = h1->weight + h2->weight;
        h3->index = h1->p = h2->p = i++;
        h3->l = h1->index; h3->r = h2->index;
        Insert(h3, H);
    }
    return OK;
}

/* generate relative huffman codes according to given huffman trees hp */
/* return OK if there is no error, otherwise return MALLOC_ERR         */
int encode_huffmantree(htnp hp, int n, hufcode *hc)
{
    int   i, j, codelen;
    htnp  h;
    UCHAR *code = (UCHAR*) malloc(n*UCHAR_SIZE);
    // there are n*UCHAR_SIZE bits at most
      
    if (code==NULL)
        return MALLOC_ERR;  

    for (i = 0; i < n; i++) {
        if (hp[i].weight == 0) {
            hc[i].len = 0;
            continue;
        }
        for (h = &hp[i], codelen = 0; h->p != UNUSED; h = &hp[h->p], ++codelen)
            code[codelen] = (h->index == hp[h->p].l) ? 0 : 1;
        if ((hc[i].codestr = (UCHAR*) malloc(codelen * UCHAR_SIZE)) == NULL)
            return MALLOC_ERR;
        hc[i].len = codelen;
        for (j = 0; j < codelen; j++)
            hc[i].codestr[j] = code[codelen-j-1]; // reverse code for decode
    }
    free(code);
    return OK;
}

/* translate chars into binary bits, and return */
UCHAR chars_to_bits(const UCHAR chars[CHAR_BITS])  
{  
    int   i;  
    UCHAR bits = 0;  
      
    bits |= chars[0];  
    for(i = 1; i < CHAR_BITS; ++i)//将8个字符转换成8个二进制位  
    {  
        bits <<= 1;  
        bits |= chars[i];       
    }  
    return bits;  
}

////////////////////////////////
/* compression implementation */
////////////////////////////////

/* initialize file ptrs of source file and obj file;         */
/* return OK if there is no error, otherwise return FILE_ERR */
int c_initial_files(char *src_filename, FILE **inp, char *obj_filename, FILE **outp)
{
    char temp_filename[MAX_NAME_LEN];

    if (src_filename == NULL)
        return FILE_ERR;
    if (obj_filename == NULL) {
        obj_filename = temp_filename;
        create_default_filename(src_filename, obj_filename);
    }
    if (strcmp(src_filename, obj_filename) == 0)
        return FILE_ERR; // rename
    printf("Uncompressed file: %s, Compressed file: %s\n", src_filename, obj_filename);

    if ((*outp = fopen(obj_filename, "wb")) == NULL)
        return FILE_ERR;
    else if ((*inp = fopen(src_filename, "rb")) == NULL)
        return FILE_ERR;

    return OK;
}

/* calculate frequency of all characters in file 'in' */
long calc_data_frequency(FILE *in, long frequency[])
{
    int   i, read_len;
    UCHAR buf[CACHE_SIZE];
    long  filesize;

    for (i = 0; i < CODE_SIZE; i++)
        frequency[i] = 0;
    fseek(in, 0L, SEEK_SET);

    do {
        read_len = fread(buf, 1, CACHE_SIZE, in);
        for (i = 0; i < read_len; i++)
            frequency[buf[i]]++;
    } while (read_len == CACHE_SIZE);

    for (i = filesize = 0; i < CODE_SIZE; i++)
        filesize += frequency[i];
    return filesize;
}

/* compress the data in file 'in' according the huffman code in hc and write it */
/* into file 'out'                                                              */
/*      ht : array of huffman tree                                              */
/*      hc : array of huffman code                                              */
int write_compress_file(FILE *in, FILE *out, htnp ht, hufcode *hc, 
                        char *src_filename, long src_filesize)
{
    int   i, child_size, zip_head = ZIP_HEAD;
    int   read_ctr, write_ctr; // read and write counter
    UCHAR write_char_ctr, code_char_ctr, copy_num;
    UCHAR read_buf[CACHE_SIZE], write_buf[CACHE_SIZE], write_chars[CHAR_BITS];
    UCHAR filename_size = strlen(src_filename);
    hufcode *cur_hc;

    fseek(in, 0L, SEEK_SET);
    fseek(out, 0L, SEEK_SET);
    // write zip head
    fwrite(&zip_head, UINT_SIZE, 1, out);
    // write src file info
    fwrite(&filename_size, UCHAR_SIZE, 1, out);
    fwrite(src_filename, sizeof(char), filename_size, out);
    fwrite(&src_filesize, sizeof(long), 1, out);
    // write huffman info
    child_size = sizeof(ht[0].l);
    for (i = CODE_SIZE; i < CODE_SIZE * 2 - 1; i++) {
        fwrite(&(ht[i].l), child_size, 1, out); // left child first
        fwrite(&(ht[i].r), child_size, 1, out);
    }
    // write coded info
    // each characters in file will be compress into less than 8 bits.
    // use write_chars to store 8 bits, when it get filled then translate it into a char
    // and store it in write_buf. when size of write_buf increase to CACHE_SIZE, write 
    // the whole write_buf into out file (read_buf with same size).
    write_ctr = write_char_ctr = 0;
    do {
        read_ctr = fread(read_buf, 1, CACHE_SIZE, in);
        for (i = 0; i < read_ctr; i++) {
            cur_hc = &hc[read_buf[i]];
            code_char_ctr = 0;
            while (code_char_ctr != cur_hc->len) {
                copy_num = (CHAR_BITS - write_char_ctr > cur_hc->len - code_char_ctr) ?
                            cur_hc->len - code_char_ctr : CHAR_BITS - write_char_ctr;
                memcpy(write_chars + write_char_ctr, cur_hc->codestr + code_char_ctr, copy_num);
                write_char_ctr += copy_num;
                code_char_ctr += copy_num;
                if (write_char_ctr != CHAR_BITS)
                    continue;
                write_char_ctr = 0;
                write_buf[write_ctr++] = chars_to_bits(write_chars);
                if (write_ctr != CACHE_SIZE)
                    continue;
                fwrite(write_buf, 1, CACHE_SIZE, out);
                write_ctr = 0;
            }
        }
    } while (read_ctr == CACHE_SIZE);
    // handle the rest buf
    fwrite(write_buf, 1, write_ctr, out);
    if (write_char_ctr != 0) {
        write_char_ctr = chars_to_bits(write_chars);
        fwrite(&write_char_ctr, 1, 1, out);
    }
    return OK;
}

int compress(char *src_filename, char *obj_filename)
{
    int     i, err_code;
    FILE    *in, *out;
    float   cmp_rate;
    htnode  ht[CODE_SIZE * 2 - 1];
    hufcode hc[CODE_SIZE];
    long    frequency[CODE_SIZE], src_filesize, obj_filesize = 0;

    err_code = c_initial_files(src_filename, &in, obj_filename, &out);
    if (err_code != OK)
        return err_code;
    puts("Files opened successfully. Start to read file info...");
    src_filesize = calc_data_frequency(in, frequency);
    puts("File read completed. Start to analyze file info...");
    printf("File size: %d bits\n", src_filesize);

    err_code = create_huffmantree(frequency, CACHE_SIZE, ht);
    if (err_code != OK)
        return err_code;

    puts("Establishion of Huffman Tree succeeded. Start huffman coding...");
    err_code = encode_huffmantree(ht, CACHE_SIZE, hc);
    if (err_code != OK)
        return err_code;

    for (i = 0; i < CODE_SIZE; i++)
        obj_filesize += frequency[i] * hc[i].len;
    obj_filesize = (obj_filesize + 7) / 8;  // convert into bits
    obj_filesize += 2 * sizeof(short) * (CODE_SIZE - 1);    // add huffman info length
    obj_filesize += strlen(src_filename) + 1;               // add src filename info
    obj_filesize += sizeof(long);                           // add src filesize info
    obj_filesize += UINT_SIZE;                              // add zip header
    cmp_rate = (float) obj_filesize / src_filesize;

    puts("Encoded successfully. Start to generate compressed file...");
    printf("Compressed file's expected size: %ld bits; Compress Rate: %%%.2lf\n", 
          obj_filesize, cmp_rate * 100);

    err_code = write_compress_file(in, out, ht, hc, src_filename, src_filesize);
    if (err_code != OK)
        return err_code;
    puts("Compression completed.");

    fclose(in);
    fclose(out);
    for (i = 0; i < CODE_SIZE; i++)
        free(hc[i].codestr);
    return OK;
}

////////////////////////////
/* Decompression functoin */
////////////////////////////

/* initialize file in ptr and file out ptr         */
/* return OK if there is no error; else return ERR */
int d_initial_files(char *src_filename, FILE **inp, char *obj_filename, FILE **outp)
{
    UINT  zip_head;
    UCHAR filename_size;
    char  temp_filename[MAX_NAME_LEN];

    printf("Uncompressed File: %s, ", src_filename);
    if ((*inp = fopen(src_filename, "rb")) == NULL)
        return FILE_ERR;
    fread(&zip_head, UINT_SIZE, 1, *inp);
    if (zip_head != ZIP_HEAD)
        return HEAD_ERR;

    fread(&filename_size, UCHAR_SIZE, 1, *inp);
    if (obj_filename == NULL) {
        obj_filename = temp_filename;
        fread(obj_filename, sizeof(char), filename_size, *inp);
        obj_filename[filename_size] = '\0';
    } else
        fseek(*inp, filename_size, SEEK_CUR);

    printf("Compression File: %s\n", obj_filename);
    if ((*outp = fopen(obj_filename, "wb")) == NULL)
        return FILE_ERR;
    return OK;
}

/* get a simple version of huffman tree from file in;  */
void get_simple_huffmantree(FILE *in, short s_ht[][2])
{
    int i;

    for (i = 0; i < CODE_SIZE; i++)
        s_ht[i][0] = s_ht[i][1] = UNUSED;
    fread(s_ht[i], sizeof(short), 2 * (CODE_SIZE-1), in);
}

/* translate huffman code in src file into normal code and write it into obj file */
/*      s_ht     : simple huffman tree (with size CODE_SIZE*2-1)                  */
/*      bits_pos : position where origin file info start                          */
int write_decompress_file(FILE *in, FILE *out, short s_ht[][2], long bits_pos, long obj_filesize)
{
    long  cur_size;
    UCHAR read_buf[CACHE_SIZE], write_buf[CACHE_SIZE], convert_bit;
    UINT  read_ctr, write_ctr, cur_pos, last_pos;

    fseek(in, bits_pos, SEEK_SET);
    fseek(out, 0L, SEEK_SET);

    read_ctr = CACHE_SIZE - 1;
    cur_size = write_ctr = 0;
    cur_pos = last_pos = find_last_sp_huffmantree(s_ht);  // find biggest tree
    while (cur_size < obj_filesize) {
        if (++read_ctr == CACHE_SIZE) {
            fread(read_buf, 1, CACHE_SIZE, in);
            read_ctr = 0;
        }

        for (convert_bit = 128; convert_bit > 0; convert_bit >>= 1) {
            cur_pos = (read_buf[read_ctr] & convert_bit) == 0 ? 
                       s_ht[cur_pos][0] : s_ht[cur_pos][1];
            if (cur_pos >= CODE_SIZE)
                continue;

            write_buf[write_ctr++] = (UCHAR) cur_pos;
            if (write_ctr == CACHE_SIZE) {
                fwrite(write_buf, 1, CACHE_SIZE, out);
                write_ctr = 0;
            }
            cur_pos = last_pos;
            if (++cur_size == obj_filesize)
                break;
        }
    }
    fwrite(write_buf, 1, write_ctr, out);
    return OK;
}

/* find last simple huffman tree in s_ht          */
/* Assume there is at least a merged huffman tree */
int find_last_sp_huffmantree(short s_ht[][2])
{
    int i;

    for (i = CODE_SIZE * 2 - 2; i >= 0; i--)
        if (s_ht[i][0] != UNUSED)
            return i;
}

/* decompress: routine to invoke relative function and for user interface */
int decompress(char *src_filename, char *obj_filename)
{
    int   err_code;
    FILE  *in, *out;
    short s_ht[CODE_SIZE * 2 - 1][2];
    long  obj_filesize;

    err_code = d_initial_files(src_filename, &in, obj_filename, &out);
    if (err_code != OK)
        return err_code;

    puts("Files opened successfully. Start to read file info...");
    fread(&obj_filesize, sizeof(long), 1, in);
    printf("Compressed File expected size: %ld\n", obj_filesize);
    puts("Start to generate Huffman Tree...");
    get_simple_huffmantree(in, s_ht);
    puts("Huffman established successfully. Start to decompress...");

    err_code = write_decompress_file(in, out, s_ht, ftell(in), obj_filesize);
    if (err_code != OK)
        return err_code;

    puts("Decompression completed.");
    fclose(in);
    fclose(out);
    return OK;
}

/////////////////////
/* helper function */
/////////////////////

char *create_default_filename(char *src_filename, char *obj_filename)
{
    char *temp;

    if ((temp = strrchr(src_filename, '.')) == NULL)
        strcpy(obj_filename, src_filename);
    else {
        strncpy(obj_filename, src_filename, temp - src_filename);
        obj_filename[temp - src_filename] = '\0';
    }
    strcat(obj_filename, ".cmp");
    return obj_filename;
}

/* for user interface */
void print_help()  
{  
    puts("\nCompress or Decompress a single file.\n");
    puts("compress [/h]");
    puts("compress [/c]|[/d] source_file [object_file]\n");
    puts("  /h\t\tdisplay help info");
    puts("  /c\t\tcompress file");
    puts("  /d\t\tdecompress file");
    puts("  source_file\t(include path)");
    puts("  object_file\t(include path)");
}

/* process the error occured during compression or decompression */
void process_error(int err_code)
{
    switch (err_code) {
        case ARGS_ERR:    puts("ERR: Wrong arguments"); break;
        case FILE_ERR:    puts("ERR: Failed to open file"); break;
        case HEAD_ERR:    puts("ERR: Illegal zip Head"); break;
        case MALLOC_ERR:  puts("ERR: Failed to allocate memory"); break;
        case HUFTREE_ERR: puts("ERR: Failed to generate huffman tree"); break;
        case HUFCODE_ERR: puts("ERR: Failed to generate huffman code"); break;
        default:          break;
    }
}

void process_args(char *first, char *second, char *third)
{
    if (strcmp(first, "/h") == 0 || strcmp(first, "/?") == 0)
        print_help();
    else if (strcmp(first, "/c") == 0)
        process_error(compress(second, third));
    else if (strcmp(first, "/d") == 0)
        process_error(decompress(second, third));
    else
        process_error(ARGS_ERR);
}

int main(int argc, char *argv[])
{
    switch (argc) {
        case   1: print_help(); break;
        case   2: process_args(argv[1], NULL, NULL); break;
        case   3: process_args(argv[1], argv[2], NULL); break;
        case   4: process_args(argv[1], argv[2], argv[3]); break;
        default : process_error(ARGS_ERR);
    }
    return OK;
}