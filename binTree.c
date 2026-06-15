#include "binTree.h"


Node *binTree_newNode(BinTree *tree, void *val);

Node *binTree_insertNode(BinTree *tree, Node *node, void *val);

Node *binTree_balance(Node *node);

Node *binTree_rotateL(Node *node);

Node *binTree_rotateR(Node *node);

i32 binTree_subHeight(Node *node);

BinTree binTree_new(BinTree_Compare *comp){
  BinTree tree = { .comp = comp };
  tree.pool = poolLink_new(sizeof(Node), 32);
  return tree;
}

Node *binTree_insertNode(BinTree *tree, Node *node, void *val){
  if(!node) return binTree_newNode(tree, val);

  int dir = tree->comp(node->val, val);

    if(!dir) return node;
    if(dir < 0)
      node->left = binTree_insertNode(tree, node->left, val);
    else if(dir > 0)
      node->right = binTree_insertNode(tree, node->right, val);

    return binTree_balance(node);
}

Node *binTree_insert(BinTree *tree, void *val){
  tree->root = binTree_insertNode(tree, tree->root, val);
}

void binTree_free(BinTree *tree){
  PoolLink *pool = tree->pool;
  tree->pool = NULL;
  tree->root = NULL;
  poolLink_free(pool);
}

Node *binTree_newNode(BinTree *tree, void *val){
  Node *node = poolLink_alloc(tree->pool);
  node->val = val;
  node->left = node->right = NULL;
  node->height = 0;
  return node;
}

Node *binTree_balance(Node *node){
  i32 lh = 0, rh = 0;
  if(node->left) lh = node->left->height;
  if(node->right) rh = node->right->height;
  i32 balance = lh - rh;
  lh = rh = 0;
  if(balance < -1){
    if(node->right->left) lh = node->right->left->height;
    if(node->right->right) rh = node->right->right->height;
    if(lh - rh > 0)
      node->right = binTree_rotateR(node->right);
    node = binTree_rotateL(node);
  }
  else if(balance > 1){
    if(node->left->left) lh = node->left->left->height;
    if(node->left->right) rh = node->left->right->height;
    if(lh - rh < 0)
      node->left = binTree_rotateL(node->left);
    node = binTree_rotateR(node);
  }
  binTree_subHeight(node);
  return node;
}

Node *binTree_rotateL(Node *node){
  Node *child = node->right, *grandChild = child->left;

  child->left = node;
  node->right = grandChild;

  return child;
}

Node *binTree_rotateR(Node *node){
  Node *child = node->left, *grandChild = child->right;

  child->right = node;
  node->left = grandChild;

  return child;
}

i32 binTree_subHeight(Node *node){
  if(!node) return 0;
  i32 lh = binTree_subHeight(node->left);
  i32 rh = binTree_subHeight(node->right);
  node->height = lh > rh ? lh : rh;
  return node->height + 1;
}