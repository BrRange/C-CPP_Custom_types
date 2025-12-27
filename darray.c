#include "darray.h"

void darray_grow(void *darrayAny, u32 target, u32 typeSize){
  darrayTemplate(void) *darray = darrayAny;
  u32 cap = darray->cap;
  cap += 2 * !cap;
  while(cap < target){
    u32 incr = cap >> 1;
    cap += incr + (incr & 1);
  }
  if(cap > darray->cap){
    darray->cap = cap;
    darray->data = REALLOC(darray->data, cap * typeSize);
  }
}

void darray_shrink(void *darrayAny, u32 typeSize){
  darrayTemplate(void) *darray = darrayAny;
  u32 cap = darray->cap;
  while(cap / 2 > darray->size) cap /= 2;
  if(cap < darray->cap){
    darray->cap = cap;
    darray->data = REALLOC(darray->data, cap * typeSize);
  }
}

void darray_append(void *darrayAny, void *element, u32 typeSize){
  darrayTemplate(void) *darray = darrayAny;
  darray_grow(darrayAny, darray->size + 1, typeSize);
  u8 (*sized)[typeSize] = darray->data;
  memcpy(sized + darray->size, element, typeSize);
  ++darray->size;
}

void darray_appendPtr(void *darrayAny, void *element){
  darray_append(darrayAny, &element, sizeof element);
}

void darray_appendMany(void *darrayAny, void *elementList, u32 elementCount, u32 typeSize){
  darrayTemplate(void) *darray = darrayAny;
  darray->size += elementCount;
  darray_grow(darray, darray->size, typeSize);
  u8 (*sized)[typeSize] = darray->data;
  memcpy(sized + darray->size - elementCount, elementList, elementCount * typeSize);
}

void darray_remove(void *darrayAny, u32 index, u32 typeSize){
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

void darray_removeMany(void *darrayAny, u32 index, u32 amount, u32 typeSize){
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

void darray_pop(void *darrayAny, u32 index, u32 typeSize){
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

void darray_popMany(void *darrayAny, u32 index, u32 amount, u32 typeSize){
  darrayTemplate(void) *darray = darrayAny;
  u8 (*sized)[typeSize] = darray->data;
  darray->size -= amount;
  u32 diff = amount + index > darray->size ? amount + index - darray->size : 0;
  if(index < darray->size)
    memcpy(
      sized + index,
      sized + darray->size + diff,
      (amount - diff) * typeSize
    );
}

void darray_destroy(void *darrayAny){
  darrayTemplate(void) *darray = darrayAny;
  free(darray->data);
  memset(darray, 0, sizeof(*darray));
}

void darray_recFree(void *darrayAny){
  darrayTemplate(void*) *darrayGenPtr = darrayAny;
  for(u32 i = 0; i < darrayGenPtr->size; ++i)
    free(darrayGenPtr->data[i]);
}