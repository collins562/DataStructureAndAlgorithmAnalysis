#include <stdio.h>
#include <stdlib.h>
#include "Huffman.h"

int main()
{
    CharSetHashTable T;
    InitCharHashTable(T);
    T[';'] = 100;
    T[' '] = 605;
    T['\n'] = 100;
    T[','] = 705;
    T['0'] = 431;
    T['1'] = 242;
    T['2'] = 176;
    T['3'] = 59;
    T['4'] = 185;
    T['5'] = 250;
    T['6'] = 174;
    T['7'] = 199;
    T['8'] = 205;
    T['9'] = 217;

    PrintWTree(Huffman(T));
}