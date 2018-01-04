#include "stack.h"
#include "get.h"
#include <stdio.h>
#include <stdlib.h>

#define MAXWORD 50

int main()
{
    char c, op2;
    char Word[MAXWORD];
    Stack S;

    S = CreateStack();
    while ((c = Getop(Word)) != EOF) {
        switch (c) {
            case NUMBER:
                Push(atoi(Word), S);
                break;
            case '+':
                Push(Pop(S)+Pop(S), S);
                break;
            case '*':
                Push(Pop(S)*Pop(S), S);
                break;
            case '-':
                op2 = Pop(S);
                Push(Pop(S)-op2, S);
                break;
            case '/':
                if ((op2 = Pop(S)) == 0)
                    fprintf(stderr, "Zero division\n");
                else
                    Push(Pop(S)/op2, S);
                break;
            case '\n':
                printf("\t%d\n", Top(S));
                break;
            default:
                fprintf(stderr, "unknown operand: %c\n", c);
                break;
        }
    }
}

