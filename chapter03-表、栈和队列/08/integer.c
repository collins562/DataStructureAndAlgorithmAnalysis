#include <stdio.h>
#include <stdlib.h>

typedef struct Node *PtrToNode;
typedef PtrToNode Digit;
typedef PtrToNode Integer;   /* Nodes sorted by represented digit   */

struct Node {
    char Num;
    PtrToNode Next;
};

Integer MakeEmpty(void);
Integer NewDigit(char, Digit);
void Insert(char, Integer, Digit);
Integer AddInt(Integer, Integer);
Integer MulInt(Integer, Integer);
Integer MulIntByDigits(Digit, Integer);
Integer MulIntByNum(char, Integer);
void PrintInt(Integer);
void PrintDigits(Digit);
Integer Pow(Integer, int);

int main()
{
    int i, Nums[10]; 
    Integer I1, I2;
    Digit Idigit;
    
    for (i = 0; i < 10; i++)
        Nums[i] = 0;

    I1 = MakeEmpty();
    Insert(2, I1, I1);      /* I1 = 2           */

    I2 = Pow(I1, 4000);     /* I2 = 2 ** 4000   */
    for (Idigit = I2->Next; Idigit != NULL; Idigit = Idigit->Next)
        Nums[Idigit->Num]++;

    for (i = 0; i < 10; i++)
        printf("%2d: %4d\n", i, Nums[i]);
}

Integer MakeEmpty(void)
{
    return NewDigit(0, NULL);
}

Digit NewDigit(char num, Digit next)
{
    Digit nD;

    if ((nD = malloc(sizeof(struct Node))) == NULL) {
        fprintf(stderr, "NewDigit: no more space.\n");
        exit(1);
    }

    nD->Num = num;
    nD->Next = next;
    return nD;
}

void Insert(char num, Integer I, Digit D)
{
    D->Next = NewDigit(num, D->Next);
}

Integer AddInt(Integer I1, Integer I2)
{
    char sum, carry = 0;
    Digit d, d1 = I1->Next, d2 = I2->Next, last;
    Integer Res = MakeEmpty();       /* new Integer representing sum */

    last = Res;
    for (; d1 != NULL && d2 != NULL; d1 = d1->Next, d2 = d2->Next) {
        if ((sum = d1->Num + d2->Num + carry) >= 10) {
            sum %= 10;
            carry = 1;
        } else
            carry = 0;
        Insert(sum, Res, last);
        last = last->Next;
    }

    d = (d1 == NULL) ? d2 : d1;

    if (carry) {
        if (d != NULL) {
            Insert(d->Num + carry, Res, last);
            d = d->Next;
        } else
            Insert(carry, Res, last);
        last = last->Next;
    }
    while (d != NULL) {
        Insert(d->Num, Res, last);
        last = last->Next;
        d = d->Next;
    }
    return Res;
}

Integer MulInt(Integer I1, Integer I2)
{
    return MulIntByDigits(I1->Next, I2);
}

Integer MulIntByDigits(Digit D, Integer I)
{
    if (D == NULL)
        return MakeEmpty();
    else
        return AddInt(MulIntByNum(D->Num, I), 
                      MulIntByNum(10, MulIntByDigits(D->Next, I)));
}

Integer MulIntByNum(char num, Integer I)
{
    char carry, product;
    Digit Idigit, Resdigit;
    Integer Res = MakeEmpty();

    carry = 0;
    Resdigit = Res;
    for (Idigit = I->Next; Idigit != NULL; Idigit = Idigit->Next) {
        if ((product = num * Idigit->Num + carry) > 9) {
            carry = product / 10;
            product %= 10;
        } else
            carry = 0;
        Insert(product, Res, Resdigit);
        Resdigit = Resdigit->Next;
    }
    /* there may be carry left in the last multiply */
    if (carry)
        Insert(carry, Res, Resdigit);

    return Res;
}

Integer Pow(Integer base, int exponent)
{
    Integer exbase, Res, TmpCell;
    Digit bfirst;

    Res = MakeEmpty();
    bfirst = base->Next;
    if (bfirst->Next == NULL && bfirst->Num == 0) {
        Insert(0, Res, Res);
        return Res;
    } else
        Insert(1, Res, Res);
    
    exbase = base;
    while (exponent > 0)
         if (exponent % 2 == 1) {
            Res = MulInt(exbase, Res);
            exponent -= 1;
        } else {
            exbase = MulInt(exbase, exbase);
            exponent /= 2;
        }

    return Res;
}

void PrintInt(Integer I)
{
    PrintDigits(I->Next);
    putchar('\n');
}

void PrintDigits(Digit D)
{
    if (D != NULL) {
        PrintDigits(D->Next);
        printf("%d", D->Num);
    }
}