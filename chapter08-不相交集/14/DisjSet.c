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
    if (S[Root1] == 0) {
        if (S[Root2] > 0)
            return SetUnion(S, Root1, S[Root2]);
        if (S[Root2] == 0) {
            S[Root1]--;
            S[Root2] = Root1;
        } else
            S[Root1] = Root2;
    } else if (S[Root2] == 0) {
        if (S[Root1] > 0)
            return SetUnion(S, S[Root1], Root2);
        S[Root2] = Root1;
    } else
        switch ( 1 * (S[Root1] > 0) + 2 * (S[Root2] > 0)) {
            case 0:
                if (S[Root1] == S[Root2]) {
                    S[Root1]--;
                    S[Root2] = Root1;
                } else if (S[Root1] > S[Root2])
                    S[Root1] = Root2;
                else
                    S[Root2] = Root1;
                break;
            case 1:
                SetUnion(S, S[Root1], Root2);
                break;
            case 2:
                SetUnion(S, Root1, S[Root2]);
                break;
            case 3:
                if (S[Root1] != S[Root2])
                    SetUnion(S, S[Root1], S[Root2]);
                break;
        }
}

SetType Find(ElementType X, DisjSet S)
{
    SetType Father, Grand;

    if (S[X] <= 0)
        return X;

    if ((Father = S[X]) < 0)
        return Father;
    if ((Grand = S[Father]) < 0)
        return Grand;
    S[X] = Grand;
    return Find(Father, S);
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