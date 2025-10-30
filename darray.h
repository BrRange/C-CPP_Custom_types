#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../rustydef.h"

#ifndef DARRAYH
#define DARRAYH

/*
Expected format:
{
    type *data;
    u32 size, cap;
    ...
}

Expected new data:
{0} || {NULL, 0, 0, ...}
*/

static void *safeRealloc(void *ptr, int targ){
  retry: void *newPtr = realloc(ptr, targ);
  if(!newPtr){
    puts("Allocation error\nRetrying...");
    goto retry;
  }
  return newPtr;
}

#define darrayTemplate(_type) struct{\
  _type *data;\
  usz size, cap;\
}

#define darrayGrow(_da, _target) do{\
  if(!(_da).cap){\
    (_da).cap = 2ull;\
    (_da).data = realloc((_da).data, sizeof(*(_da).data) * 2);\
    break;\
  }\
  bool _changed = false;\
  while((da).cap < (_target)){\
    (da).cap *= 2;\
    _changed =  true;\
  }\
  (_da).data = realloc((_da).data, sizeof(*(_da).data) * (_da).cap);\
} while(0)

void darray_grow(void *darrayGeneric, usz target, usz typeSize);

#define darrayShrink(_da) do{\
  while(((_da).cap >> 1ull) > (_da).size) (_da).cap >>= 1ull;\
  (_da).data = safeRealloc((_da).data, sizeof(*(_da).data) * (_da).cap);\
} while(0)

void darray_shrink(void *darrayGenereric, usz typeSize);

#define darrayAppend(_da, _el) do{\
  darrayGrow(_da, (_da).size + 1)\
  (_da).data[(_da).size] = (_el);\
  ++(_da).size;\
} while(0)

void darray_append(void *darrayGeneric, void *element, usz typeSize);

void darray_appendPtr(void *darrayGeneric, void *element);

#define darrayAppendMany(_da, _li, _n) do{\
  (_da).size += (_n);\
  darrayGrow(_da, (_da).size);\
  memcpy((_da).data + (_da).size - _n, _li, _n * sizeof(*(_da).data));\
} while(0)

void darray_appendMany(void *darrayGeneric, void *elementList, usz elementCount, usz typeSize);

#define darrayRemove(_da, index) do{\
  if(index < (_da).size - 1)\
    memmove(\
      (_da).data + (index),\
      (_da).data + (index) + 1,\
      ((_da).size - (index) - 1) * sizeof(*(_da).data)\
    );\
  --(_da).size;\
} while(0)

void darray_remove(void *darrayGeneric, usz index, usz typeSize);

#define darrayRemoveMany(_da, _index, _amount) do{\
  if(_index < (_da).size - _amount)\
    memmove(\
      (_da).data + (_index),\
      (_da).data + (_index) + (_amount),\
      ((_da).size - (_index) - (_amount)) * sizeof(*(_da).data)\
    );\
  (_da).size -= (_amount);\
} while(0)

void darray_removeMany(void *darrayGeneric, usz index, usz amount, usz typeSize);

#define darrayDestroy(_da) do{\
  free((_da).data);\
  memset(&(_da), 0, sizeof(_da));\
} while(0)

void darray_destroy(void *darrayGeneric);

#define darrayRecFree(_da) do{\
  for(usz i = 0; i < (_da).size; ++i)\
    free((_da).data[i]);\
} while(0)

void darray_recFree(void *darrayGeneric);

#define darrayInterate(_da, _type)\
  for(usz _i = 0; _i < (_da).size; ++_i)\
  for(_type _el = (_da).data[_i], *_once = NULL + 1; _once; _once = NULL)

#endif