#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define TestNum     100
#define LowerBound  1000
#define UpperBound  100000

double test_pairheap(int N, int TestTimes);
double test_pairheap_with_dec_key(int N, int TestTimes);
double test_pairheap_aux(int N, int TestTimes);
double test_pairheap_aux_with_dec_key(int N, int TestTimes);

int main()
{
    int i;

    printf("\nPairHeap Test Without Decrease_Key:\n\n");
    printf("|        | pairheap | pairheap_aux |\n");
    for (i = LowerBound; i <= UpperBound; i *= 10)
        printf("|%7d | %.5fs |   %.5fs   |\n", i, test_pairheap(i, TestNum),
                                                  test_pairheap_aux(i, TestNum));
    printf("\nPairHeap Test With Decrease_Key:\n\n");
    printf("|        | pairheap | pairheap_aux |\n");
    for (i = LowerBound; i <= UpperBound; i *= 10)
        printf("|%7d | %.5fs |   %.5fs   |\n", i, test_pairheap_with_dec_key(i, TestNum),
                                                  test_pairheap_aux_with_dec_key(i, TestNum));
}