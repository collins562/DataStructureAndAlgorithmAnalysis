#ifndef _Sort_H

typedef int ElementType;

void InsertionSort(ElementType A[], int N);
void ShellSort(ElementType A[], int N);
void HeapSort(ElementType A[], int N);
void MergeSort(ElementType A[], int N);
void QuickSort(ElementType A[], int N);
ElementType QuickSelect(ElementType A[], int k, int N);
void PrintArray(ElementType A[], int N);

#endif