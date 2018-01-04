#include <stdio.h>
#include <stdlib.h>
#include "ExHash.h"

int main()
{
    int i;
    int nums[] = { 0b10111101, 0b00000010, 0b10011011, 0b10111110, 0b01111111,
                   0b01010001, 0b10010110, 0b00001011, 0b11001111, 0b10011110,
                   0b11011011, 0b00101011, 0b01100001, 0b11110000, 0b01101111 };
    HashTable H;

    H = InitializeTable(8);
    for (i = 0; i < sizeof(nums)/sizeof(nums[0]); i++) {
        Insert(nums[i], H);
        PrintHashTable(H);
    }

}