#ifndef _DisjSet_H

#ifndef NumSet
#define NumSet 9
#endif

typedef int DisjSet[NumSet + 1];
typedef int SetType;
typedef int ElementType;

void Initialize(DisjSet S);
void SetUnion(DisjSet S, SetType Root1, SetType Root2);
SetType Find(ElementType X, DisjSet S);
void PrintSet(DisjSet S);

#endif