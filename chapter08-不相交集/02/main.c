#include <stdio.h>
#include "DisjSet.h"

int main()
{
    DisjSet S;

    Initialize(S);
    PrintSet(S);
    SetUnion(S, 1, 2);
    SetUnion(S, 3, 4);
    SetUnion(S, 3, 5);
    SetUnion(S, 1, 7);
    SetUnion(S, 3, 6);
    SetUnion(S, 8, 9);
    SetUnion(S, 1, 8);
    SetUnion(S, 3, 10);
    SetUnion(S, 3, 11);
    SetUnion(S, 3, 12);
    SetUnion(S, 3, 13);
    SetUnion(S, 14, 15);
    SetUnion(S, 16, 17);
    SetUnion(S, 14, 16);
    SetUnion(S, 1, 3);
    SetUnion(S, 1, 14);
    PrintSet(S);
    Find(17, S);
    PrintSet(S);
}