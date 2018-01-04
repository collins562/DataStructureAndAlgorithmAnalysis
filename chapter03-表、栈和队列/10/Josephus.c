#include <stdio.h>
#include <stdlib.h>
#include "clist.h"

struct Node {
    ElementType Element;
    Position    Previous;
    Position    Next;
};

int Josephus(int, int);
Position UpdatePos(Position, CircleList);

int main()
{
    printf("Josephus(N: 5, M: 0): %d\n", Josephus(5, 0));
    printf("Josephus(N: 5, M: 1): %d\n", Josephus(5, 1));
}

int Josephus(int N, int M)
{
    int i;
    CircleList circle;
    Position P, Tmp;

    circle = MakeEmpty(NULL);
    for (i = N; i > 0; i--)
        Insert(i, circle, circle);

    P = First(circle);
    while (Length(circle) > 1) {
        for (i = M % Length(circle); i > 0; i--)
            P = UpdatePos(P, circle);

        Tmp = UpdatePos(P, circle);
        P->Previous->Next = P->Next;
        P->Next->Previous = P->Previous;
        free(P);
        P = Tmp;
        circle->Element -= 1;
    }

    return First(circle)->Element;
}

Position UpdatePos(Position P, CircleList CL)
{
    return (IsLast(P, CL)) ? First(CL) : P->Next;
}