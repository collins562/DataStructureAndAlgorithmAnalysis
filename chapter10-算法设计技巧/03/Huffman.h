#ifndef _Huffman_H

struct TreeNode;
struct WeightTreeRecord;
typedef struct TreeNode *PtrToTree;
typedef struct WeightTreeRecord *WTree;
typedef int CharSetHashTable[128];

void InitCharHashTable(CharSetHashTable T);
WTree CreateWTree(char C, int Weight);
void DisposeWTree(WTree T);
WTree Huffman(CharSetHashTable T);
void PrintWTree(WTree T);
int WTreecmp(WTree T1, WTree T2);

#endif