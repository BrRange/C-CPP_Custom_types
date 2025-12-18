#include "darray.h"

void darray_grow(void *darrayAny, usz target, usz typeSize){
  darrayTemplate(void) *darray = darrayAny;
  bool changed = false;
  if(!darray->cap){
    darray->cap = 2;
    changed = true;
  }
  while(darray->cap < target){
    usz incr = darray->cap >> 1;
    darray->cap += incr + (incr & 1);
    changed = true;
  }
  if(changed) darray->data = realloc(darray->data, darray->cap * typeSize);
}

void darray_shrink(void *darrayAny, usz typeSize){
  darrayTemplate(void) *darray = darrayAny;
  while(darray->cap / 2 > darray->size) darray->cap /= 2;
  darray->data = realloc(darray->data, typeSize * darray->cap);
}

void darray_append(void *darrayAny, void *element, usz typeSize){
  darrayTemplate(void) *darray = darrayAny;
  darray_grow(darrayAny, darray->size + 1, typeSize);
  u8 (*sized)[typeSize] = darray->data;
  memcpy(sized + darray->size, element, typeSize);
  ++darray->size;
}

void darray_appendPtr(void *darrayAny, void *element){
  darray_append(darrayAny, &element, sizeof element);
}

void darray_appendMany(void *darrayAny, void *elementList, usz elementCount, usz typeSize){
  darrayTemplate(void) *darray = darrayAny;
  darray->size += elementCount;
  darray_grow(darray, darray->size, typeSize);
  u8 (*sized)[typeSize] = darray->data;
  memcpy(sized + darray->size - elementCount, elementList, elementCount * typeSize);
}

void darray_remove(void *darrayAny, usz index, usz typeSize){
  darrayTemplate(void) *darray = darrayAny;
  u8 (*sized)[typeSize] = darray->data;
  --darray->size;
  if(index < darray->size)
    memmove(
      sized + index,
      sized + index + 1,
      (darray->size - index) * typeSize
    );
}

void darray_removeMany(void *darrayAny, usz index, usz amount, usz typeSize){
  darrayTemplate(void) *darray = darrayAny;
  u8 (*sized)[typeSize] = darray->data;
  darray->size -= amount;
  if(index < darray->size)
    memmove(
      sized + index,
      sized + index + amount,
      (darray->size - index) * typeSize
    );
}

void darray_pop(void *darrayAny, usz index, usz typeSize){
  darrayTemplate(void) *darray = darrayAny;
  u8 (*sized)[typeSize] = darray->data;
  --darray->size;
  if(index < darray->size)
    memcpy(
      sized + index,
      sized + darray->size,
      typeSize
    );
}

void darray_destroy(void *darrayAny){
  darrayTemplate(void) *darray = darrayAny;
  free(darray->data);
  memset(darray, 0, sizeof(*darray));
}

void darray_recFree(void *darrayAny){
  darrayTemplate(void*) *darrayGenPtr = darrayAny;
  for(usz i = 0; i < darrayGenPtr->size; ++i)
    free(darrayGenPtr->data[i]);
}