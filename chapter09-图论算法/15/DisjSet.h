#ifndef _DisjSet_H

#ifndef NumSet
#define NumSet 32
#endif

typedef int DisjSet[NumSet + 1];
typedef int SetType;
typedef int SetElementType;

void InitSet(DisjSet S);
void SetUnion(DisjSet S, SetType Root1, SetType Root2);
SetType Find(SetElementType X, DisjSet S);
void PrintSet(DisjSet S);

#endif