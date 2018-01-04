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
    char c;

    while (isspace(*s = c = Getch()) && c != '\n')
        ;

    if (!isdigit(c))
        return c;
    
    while (isdigit(*++s = c = Getch()))
        ;
    UnGetch(c);
    *s = '\0';
    return NUMBER;
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