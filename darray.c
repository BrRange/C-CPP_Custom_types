#include "darray.h"

void darray_grow(void *darray, usz target, usz typeSize){
  darrayTemplate(void) *darrayGen = darray;
  bool changed = false;
  if(!darrayGen->cap){
    darrayGen->cap = 2;
    changed = true;
  }
  while(darrayGen->cap < target){
    darrayGen->cap *= 2;
    changed = true;
  }
  if(changed) darrayGen->data = REALLOC(darrayGen->data, darrayGen->cap * typeSize);
}

void darray_shrink(void *darray, usz typeSize){
  darrayTemplate(void) *darrayGen = darray;
  while(darrayGen->cap / 2 > darrayGen->size) darrayGen->cap /= 2;
  darrayGen->data = REALLOC(darrayGen->data, typeSize * darrayGen->cap);
}

void darray_append(void *darray, void *element, usz typeSize){
  darrayTemplate(void) *darrayGen = darray;
  darray_grow(darray, darrayGen->size + 1, typeSize);
  u8 (*sized)[typeSize] = darrayGen->data;
  memcpy(sized + darrayGen->size, element, typeSize);
  ++darrayGen->size;
}

void darray_appendPtr(void *darray, void *element){
  darray_append(darray, &element, sizeof element);
}

void darray_appendMany(void *darray, void *elementList, usz elementCount, usz typeSize){
  darrayTemplate(void) *darrayGen = darray;
  darrayGen->size += elementCount;
  darray_grow(darrayGen, darrayGen->size, typeSize);
  u8 (*sized)[typeSize] = darrayGen->data;
  memcpy(sized + darrayGen->size - elementCount, elementList, elementCount * typeSize);
}

void darray_remove(void *darray, usz index, usz typeSize){
  darrayTemplate(void) *darrayGen = darray;
  u8 (*sized)[typeSize] = darrayGen->data;
  if(index < darrayGen->size - 1)
    memmove(
      sized + index,
      sized + index + 1,
      (darrayGen->size - index - 1) * typeSize
    );
  --darrayGen->size;
}

void darray_removeMany(void *darray, usz index, usz amount, usz typeSize){
  darrayTemplate(void) *darrayGen = darray;
  u8 (*sized)[typeSize] = darrayGen->data;
  if(index < darrayGen->size - amount)
    memmove(
      sized + index,
      sized + index + amount,
      (darrayGen->size - index - amount) * typeSize
    );
  darrayGen->size -= amount;
}

void darray_destroy(void *darray){
  darrayTemplate(void) *darrayGen = darray;
  free(darrayGen->data);
  memset(darrayGen, 0, sizeof(*darrayGen));
}

void darray_recFree(void *darray){
  darrayTemplate(void*) *darrayGenPtr = darray;
  for(usz i = 0; i < darrayGenPtr->size; ++i)
    free(darrayGenPtr->data[i]);
}