#include "stack.h"
#include "get.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXWORD 50

void Operate(char, char *, Stack);
char *Bracket(char *);
int ContainOps(char *, char *);
void PostToIn(void);

int main()
{
    PostToIn();
}

void PostToIn(void)
{
    char c, *exp;
    char Word[MAXWORD];
    Stack S;

    S = CreateStack();
    while ((c = Getop(Word)) != EOF) {
        switch (c) {
            case NUMBER:
            case NAME:
                Push(Word, S);
                break;
            case '+':
            case '-':
            case '*':
            case '/':
            case '^':
                Operate(c, exp, S);
                break;
            case '\n':
                printf("infix: %s\n", Pop(S));
                break;
            default:
                Error("unknown operand");
                break;
        }
    }
}

void Operate(char op, char *exp, Stack S)
{
    char *operand1, *operand2;
    char Tmp[100];

    operand2 = Pop(S);
    operand1 = Pop(S);
    if (operand2 == EOS || operand1 == EOS) {
        fprintf(stderr, "Operate: not enough operands.\n");
        exit(1);
    }
    if (op == '*' || op == '/') {
        if (ContainOps(operand1, "+-")) 
            operand1 = Bracket(operand1);
        if (ContainOps(operand2, "+-"))
            operand2 = Bracket(operand2);
    } else if (op == '^') {
        if (ContainOps(operand1, "+-*/"))
            operand1 = Bracket(operand1);
        if (ContainOps(operand2, "+-*/"))
            operand2 = Bracket(operand2);
    }

    sprintf(exp, "%s %c %s", operand1, op, operand2);
    Push(exp, S);
}

char *Bracket(char *s)
{
    char Tmp[100];

    sprintf(Tmp, "(%s)", s);
    return s = Tmp;
}

int ContainOps(char *s, char *ops)
{
    for (; *s != '\0' && strchr(ops, *s) == NULL; s++)
        ;
    return *s != '\0';
}