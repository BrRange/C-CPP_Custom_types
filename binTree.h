#ifndef BINARY_TREE_H_
#define BINARY_TREE_H_

#include <stdlib.h>
#include "pool.h"

typedef struct Node Node;
struct Node{
  void *val;
  Node *left, *right; 
  i32 height;
};

typedef int BinTree_Compare(const void *, const void *);

typedef struct BinTree{
  PoolLink *pool;
  Node *root;
  BinTree_Compare *comp;
} BinTree;

BinTree binTree_new(BinTree_Compare *comp);

Node *binTree_insert(BinTree *tree, void *val);

void binTree_free(BinTree *tree);

#endif