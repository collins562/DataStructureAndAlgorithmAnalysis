#include <stdio.h>
#include <stdlib.h>
#include "HashQuad.h"

int main()
{
    int i;
    int nums[] = { 4371, 1323, 6173, 4199, 4344, 9679, 1989 };
    HashTable H;

    H = InitializeTable(10);
    for (i = 0; i < sizeof(nums)/sizeof(nums[0]); i++)
        Insert(nums[i], H);

    PrintHashTable(H);

    H = Rehash(H);
    PrintHashTable(H);
}