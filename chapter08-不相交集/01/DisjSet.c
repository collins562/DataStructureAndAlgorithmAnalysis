#include <stdio.h>
#include <stdlib.h>
#include "DisjSet.h"

#define Error(Message) fprintf(stderr, "%s\n", Message)

void Initialize(DisjSet S)
{
    int i;

    for (i = NumSet; i > 0; i--)
        S[i] = 0;
}

/* Assume Root1 and Root2 are roots. union is a C keyword, so this  */
/* is named SetUnion                                                */
void SetUnion (DisjSet S, SetType Root1, SetType Root2)
{
    if (S[Root2] < S[Root1])    /* Root2 is deeper Set              */
        S[Root1] = Root2;       /* Make Root2 new root              */
    else {
        if (S[Root1] == S[Root2])
            S[Root1]--;
        S[Root2] = Root1;
    }
}

SetType Find(ElementType X, DisjSet S)
{
    if (S[X] <= 0)
        return X;
    else
        return S[X] = Find(S[X], S);
}

void PrintSet(DisjSet S)
{
    int i;

    for (i = 1; i <= NumSet; i++)
        printf(" %02d |", i);
    putchar('\n');
    for (i = 1; i <= NumSet; i++)
        if (S[i] < 0)
            printf(" %d |", S[i]);
        else
            printf(" %02d |", S[i]);
    printf("\n\n");
}