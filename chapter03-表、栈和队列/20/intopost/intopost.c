#include "stack.h"
#include "get.h"
#include <stdio.h>
#include <stdlib.h>

#define MAXWORD 50

int Priority(char);
void InToPost(void);

int main()
{
    InToPost();
}

void InToPost(void)
{
    char c, op;
    char Word[MAXWORD];
    Stack S;

    S = CreateStack();
    while ((c = Getop(Word)) != EOF) {
        switch (c) {
            case NUMBER:
            case NAME:
                printf("%s ", Word);
                break;
            case '+':
            case '-':
            case '*':
            case '/':
            case '^':
            case '(':
                while (Top(S) != '(' && Top(S) != EOS &&
                       Priority(c) <= Priority(Top(S)))
                    printf("%c ", Pop(S));
                Push(c, S);
                break;
            case ')':
                while ((op = Pop(S)) != '(' && op != EOS)
                    printf("%c ", op);
                if (op == EOS)
                    Error("\n\tExpect '(' before ')");
                break;
            case '\n':
                while ((op = Pop(S)) != EOS) {
                    if (op == '(')
                        Error("\n\tExpect ')' after '(");
                    printf("%c ", op);
                }
                putchar('\n');
                break;
            default:
                Error("unknown operand");
                break;
        }
    }
}

int Priority(char op)
{
    if (op == '(' || op == ')')
        return 10;
    else if (op == '^')
        return 3;
    else if (op == '*' || op == '/')
        return 2;
    else if (op == '+' || op == '-')
        return 1;
    else
        return 0;
}