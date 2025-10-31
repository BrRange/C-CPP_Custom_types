#include "darray.h"

void darray_grow(void *darrayGeneric, usz target, usz typeSize){
  darrayTemplate(void) *darray = darrayGeneric;
  bool changed = false;
  if(!darray->cap){
    darray->cap = 2;
    changed = true;
  }
  while(darray->cap < target){
    darray->cap *= 2;
    changed = true;
  }
  if(changed) darray->data = safeRealloc(darray->data, darray->cap * typeSize);
}

void darray_shrink(void *darrayGenereric, usz typeSize){
  darrayTemplate(void) *darray = darrayGenereric;
  while(darray->cap / 2 > darray->size) darray->cap /= 2;
  darray->data = safeRealloc(darray->data, typeSize * darray->cap);
}

void darray_append(void *darrayGeneric, void *element, usz typeSize){
  darrayTemplate(void) *darray = darrayGeneric;
  darray_grow(darrayGeneric, darray->size + 1, typeSize);
  u8 (*sized)[typeSize] = darray->data;
  memcpy(sized + darray->size, element, typeSize);
  ++darray->size;
}

void darray_appendPtr(void *darrayGeneric, void *element){
  darray_append(darrayGeneric, &element, sizeof element);
}

void darray_appendMany(void *darrayGeneric, void *elementList, usz elementCount, usz typeSize){
  darrayTemplate(void) *darray = darrayGeneric;
  darray->size += elementCount;
  darray_grow(darray, darray->size, typeSize);\
  u8 (*sized)[typeSize] = darray->data;
  memcpy(sized + darray->size - elementCount, elementList, elementCount * typeSize);
}

void darray_remove(void *darrayGeneric, usz index, usz typeSize){
  darrayTemplate(void) *darray = darrayGeneric;
  u8 (*sized)[typeSize] = darray->data;
  if(index < darray->size - 1)
    memmove(
      sized + index,
      sized + index + 1,
      (darray->size - index - 1) * typeSize\
    );
  --darray->size;
}

void darray_removeMany(void *darrayGeneric, usz index, usz amount, usz typeSize){
  darrayTemplate(void) *darray = darrayGeneric;
  u8 (*sized)[typeSize] = darray->data;
  if(index < darray->size - amount)\
    memmove(\
      sized + index,\
      sized + index + amount,\
      (darray->size - index - amount) * typeSize\
    );\
  darray->size -= amount;\
}

void darray_destroy(void *darrayGeneric){
  darrayTemplate(void) *darray = darrayGeneric;
  free(darray->data);
  memset(darray, 0, sizeof(*darray));
}

void darray_recFree(void *darrayGeneric){
  darrayTemplate(void*) *darray = darrayGeneric;
  for(usz i = 0; i < darray->size; ++i)
    free(darray->data[i]);
}