#ifndef _ExHash_H

struct Point;
typedef struct Point Position;

struct HashTbl;
typedef struct HashTbl *HashTable;
typedef int ElementType;

HashTable InitializeTable(int TableSize);
void DestroyTable(HashTable H);
Position Find(ElementType Key, HashTable H);
void Insert(ElementType Key, HashTable H);
ElementType Retrieve(Position P, HashTable H);
HashTable Rehash(HashTable H);
void PrintHashTable(HashTable H);
void Error(char *Message);
/* Routines such as Delete and MakeEmpty are omitted */

#endif