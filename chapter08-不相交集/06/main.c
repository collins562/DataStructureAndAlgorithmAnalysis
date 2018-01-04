#include <stdio.h>
#include "DisjSet.h"

int main()
{
    DisjSet S;

    Initialize(S);
    PrintSet(S);
    SetUnion(S, 1, 2);
    PrintSet(S);
    SetUnion(S, 3, 4);
    PrintSet(S);
    SetUnion(S, 3, 6);
    PrintSet(S);
    SetUnion(S, 5, 7);
    PrintSet(S);
    SetUnion(S, 4, 6);
    PrintSet(S);
    SetUnion(S, 2, 4);
    PrintSet(S);
    SetUnion(S, 8, 9);
    PrintSet(S);
    SetUnion(S, 5, 8);
    PrintSet(S);
}