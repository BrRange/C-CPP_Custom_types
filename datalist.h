#ifndef DATA_LIST_H
#define DATA_LIST_H

#include "rustydef.h"

typedef struct LNode{
  void *data;
  struct LNode *next;
} LNode;

typedef struct LList{
  LNode *root;
} LList;

typedef int (*ListCompare)(void *target, void *data);

void llist_push(LList *ll, void *data);

void llist_pop(LList *ll, LNode *node);

LNode *llist_find(LList *ll, void *data, ListCompare cmp);

int llist_findPop(LList *ll, void *data, ListCompare cmp);

void llist_free(LList *ll);

typedef struct XNode{
  void *data;
  struct XNode *xor;
} XNode;

typedef struct XList{
  XNode *root;
} XList;

typedef struct XIterator{
  XNode *prev, *curr, *next;
} XIterator;

/**
 * @brief Creates an iterator starting at the list's root
 * @return A complete iterator
 */
XIterator xlist_newIterator(XList *xl);

/**
 * @brief Creates an iterator based on a node and it's predecessor
 * @return A complete iterator
 */
XIterator xlist_newIteratorPrev(XNode *curr, XNode *prev);

/**
 * @brief Creates an iterator based on a node and it's successor
 * @return A complete iterator
 */
XIterator xlist_newIteratorNext(XNode *curr, XNode *next);

/**
 * @brief Advances the iterator if possible
 * @return If the iterator changed
 */
int xlist_iteratorNext(XIterator *xi);

/**
 * @brief Retreats the iterator if possible
 * @return If the iterator changed
 */
int xlist_iteratorPrev(XIterator *xi);

void xlist_push(XList *xl, void *data);

/**
 * @brief Frees the current node and stitches the remaining list
 * @param xi Iterador with the desired node at the current position returning at the previous node if possible
 */
void xlist_pop(XList *xl, XIterator *xi);

XIterator xlist_find(XList *xl, void *data, ListCompare cmp);

void xlist_free(XList *xl);

typedef struct DNode{
  void *data;
  struct DNode *next, *prev;
} DNode;

typedef struct DList{
  DNode *root;
} DList;

void dlist_push(DList *dl, void *data);

void dlist_pop(DList *dl, DNode *node);

DNode *dlist_find(DList *dl, void *data, ListCompare cmp);

void dlist_free(DList *dl);

#endif