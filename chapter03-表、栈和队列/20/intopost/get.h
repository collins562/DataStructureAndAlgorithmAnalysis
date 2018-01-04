#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXBUF  50
#define NUMBER  '0'
#define NAME    'a'

char Getop(char *);
char Getch(void);
void UnGetch(char);

static char BUF[MAXBUF];
static char *bufp = BUF;

char Getop(char *s)
{
    char c, type;

    while (isspace(*s = c = Getch()) && c != '\n')
        ;

    if (!isalnum(c) && c != '_')
        return c;
    
    if (isalpha(c) || c == '_') {
        while (isalnum(*++s = c = Getch()) || c == '_')
            ;
        type = NAME;
    } else {
        while (isdigit(*++s = c = Getch()))
            ;
        type = NUMBER;
    }

    if (c != EOF)
        UnGetch(c);
    *s = '\0';
    return type;
}

char Getch(void)
{
    return (bufp > BUF) ? *(--bufp) : getchar();
}

void UnGetch(char c)
{
    if (bufp - BUF > MAXBUF)
        fprintf(stderr, "UnGetch: out of space\n");
    else
        *(bufp++) = c;
}