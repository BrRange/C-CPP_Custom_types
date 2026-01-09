#include "datalist.h"

#include <stdlib.h>

LNode *llist_newNode(void *data){
  LNode *node = malloc(sizeof *node);
  node->data = data;
  node->next = NULL;
  return node;
}

void llist_push(LList *restrict ll, void *data){
  LNode *node = ll->root;
  ll->root = llist_newNode(data);
  ll->root->next = node;
}

void llist_pop(LList *restrict ll, LNode *node){
  if(ll->root == node){
    node = ll->root->next;
    free(ll->root);
    ll->root = node;
    return;
  }
  LNode *temp = ll->root;
  while(temp->next != node) temp = temp->next;
  node = temp->next->next;
  free(temp->next);
  temp->next = node;
}

LNode *llist_find(LList *restrict ll, void *data, ListCompare cmp){
  LNode *node = ll->root;
  while(node){
    if(cmp(data, node->data)) return node;
    node = node->next;
  }
  return NULL;
}

int llist_findPop(LList *restrict ll, void *data, ListCompare cmp){
  if(!ll->root) return 0;
  LNode *node;
  if(cmp(data, ll->root->data)){
    node = ll->root->next;
    free(ll->root);
    ll->root = node;
    return 1;
  }
  node = ll->root;
  while(node->next){
    if(cmp(data, node->next->data)){
      LNode *followUp = node->next->next;
      free(node->next);
      node->next = followUp;
      return 1;
    }
    node = node->next;
  }
  return 0;
}

void llist_free(LList *ll){
  LNode *node = ll->root, *compute;
  ll->root = NULL;
  while(node){
    compute = node;
    node = node->next;
    free(compute);
  }
}

XNode *xlist_xor(XNode *a, XNode *b){
  return (XNode*)((usz)a ^ (usz)b);
}

XIterator xlist_newIterator(XList *xl){
  if(!xl->root) return (XIterator){0};
  XIterator xi = {
    .prev = NULL,
    .curr = xl->root,
    .next = xl->root->xor
  };
  return xi;
}

XIterator xlist_newIteratorPrev(XNode *curr, XNode *prev){
  XIterator xi = {
    .prev = prev,
    .curr = curr,
    .next = xlist_xor(curr->xor, prev),
  };
  return xi;
}

XIterator xlist_newIteratorNext(XNode *curr, XNode *next){
  XIterator xi = {
    .next = next,
    .curr = curr,
  };
  xi.prev = xlist_xor(curr->xor, next);
  return xi;
}

int xlist_iteratorNext(XIterator *xi){
  if(!xi->next) return 0;
  *xi = xlist_newIteratorPrev(xi->next, xi->curr);
  return 1;
}

int xlist_iteratorPrev(XIterator *xi){
  if(!xi->prev) return 0;
  *xi = xlist_newIteratorNext(xi->prev, xi->curr);
  return 1;
}

XNode *xlist_newNode(void *data){
  XNode *node = malloc(sizeof *node);
  node->data = data;
  node->xor = 0;
  return node;
}

void xlist_push(XList *restrict xl, void *data){
  XNode *node = xl->root;
  xl->root = xlist_newNode(data);
  xl->root->xor = xlist_xor(xl->root->xor, node);
  if(node) node->xor = xlist_xor(node->xor, xl->root);
}

void xlist_pop(XList *restrict xl, XIterator *xi){
  free(xi->curr);
  if(!xi->prev && !xi->next){
    xl->root = NULL;
    xi->curr = NULL;
    return;
  }
  if(!xi->prev){
    xl->root = xi->next;
    xl->root->xor = xlist_xor(xl->root->xor, xi->curr);
    *xi = xlist_newIterator(xl);
    return;
  }
  if(!xi->next){
    xi->prev->xor = xlist_xor(xi->prev->xor, xi->curr);
    *xi = xlist_newIteratorNext(xi->prev, NULL);
    return;
  }
  xi->prev->xor = xlist_xor(xi->prev->xor, xlist_xor(xi->curr, xi->next));
  xi->next->xor = xlist_xor(xi->next->xor, xlist_xor(xi->curr, xi->prev));
  *xi = xlist_newIteratorNext(xi->prev, xi->next);
}

XIterator xlist_find(XList *restrict xl, void *data, ListCompare cmp){
  XIterator xi = xlist_newIterator(xl);
  if(!xi.curr) return xi;
  do if(cmp(data, xi.curr->data)) return xi;
  while(xlist_iteratorNext(&xi));
  return (XIterator){0};
}

void xlist_free(XList *xl){
  XIterator xi = xlist_newIterator(xl);
  xl->root = NULL;
  while(xlist_iteratorNext(&xi))
    free(xi.prev);
  free(xi.curr);
}

DNode *dlist_newNode(void *data){
  DNode *node = malloc(sizeof *node);
  node->data = data;
  node->prev = node->next = NULL;
  return node;
}

void dlist_push(DList *restrict dl, void *data){
  DNode *node = dl->root;
  dl->root = dlist_newNode(data);
  if(node){
    dl->root->next = node;
    node->prev = dl->root;
  }
}

void dlist_pop(DList *restrict dl, DNode *node){
  DNode *temp;
  if(dl->root == node){
    node = dl->root->next;
    free(dl->root);
    dl->root = node;
    if(node) node->prev = NULL;
    return;
  }
  node->prev->next = node->next;
  if(node->next) node->next->prev = node->prev;
  free(node);
}

DNode *dlist_find(DList *restrict dl, void *data, ListCompare cmp){
  DNode *node = dl->root;
  if(!node) return NULL;
  do if(cmp(data, node->data)) return node;
  while(node = node->next);
  return NULL;
}

void dlist_free(DList *dl){
  LNode *node = (LNode*)dl->root, *compute;
  dl->root = NULL;
  while(node){
    compute = node;
    node = node->next;
    free(compute);
  }
}