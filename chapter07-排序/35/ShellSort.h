#ifndef _Sort_H

typedef int ElementType;

void ShellSort1(ElementType A[], int N); /* original */
void ShellSort2(ElementType A[], int N); /* Hibbard inc */
void ShellSort3(ElementType A[], int N); /* Knuth inc */
void ShellSort4(ElementType A[], int N); /* Gonnet inc */
void ShellSort5(ElementType A[], int N); /* Sedgewich inc */
void PrintArray(ElementType A[], int N);

#endif