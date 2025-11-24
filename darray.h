#ifndef DYNAMIC_ARRAY_H_
#define DYNAMIC_ARRAY_H_

#include <stdlib.h>
#include <string.h>
#include "rustydef.h"

/*
Expected format:
{
    type *data;
    usz size, cap;
    ...
}

Expected new data:
{0} || {NULL, 0, 0, ...}
*/

static void *safeRealloc(void *ptr, int targ){
  retry: void *newPtr = realloc(ptr, targ);
  if(!newPtr) goto retry;
  return newPtr;
}

#define darrayTemplate(_type) struct{\
  _type *data;\
  usz size, cap;\
}

#define darrayGrow(_da, _target) do{\
  bool _changed = false;\
  if(!(_da).cap){\
    (_da).cap = 2ull;\
    _changed = true;\
  }\
  while((_da).cap < (_target)){\
    (_da).cap *= 2;\
    _changed =  true;\
  }\
  (_da).data = realloc((_da).data, sizeof(*(_da).data) * (_da).cap);\
} while(0)

void darray_grow(void *darrayAny, usz target, usz typeSize);

#define darrayShrink(_da) do{\
  while(((_da).cap >> 1ull) > (_da).size) (_da).cap >>= 1ull;\
  (_da).data = safeRealloc((_da).data, sizeof(*(_da).data) * (_da).cap);\
} while(0)

void darray_shrink(void *darrayAny, usz typeSize);

#define darrayAppend(_da, _el) do{\
  darrayGrow(_da, (_da).size + 1);\
  (_da).data[(_da).size] = (_el);\
  ++(_da).size;\
} while(0)

void darray_append(void *darrayAny, void *element, usz typeSize);

void darray_appendPtr(void *darrayAny, void *element);

#define darrayAppendMany(_da, _li, _n) do{\
  (_da).size += (_n);\
  darrayGrow(_da, (_da).size);\
  memcpy((_da).data + (_da).size - _n, _li, _n * sizeof(*(_da).data));\
} while(0)

void darray_appendMany(void *darrayAny, void *elementList, usz elementCount, usz typeSize);

#define darrayRemove(_da, index) do{\
  if(index < (_da).size - 1)\
    memmove(\
      (_da).data + (index),\
      (_da).data + (index) + 1,\
      ((_da).size - (index) - 1) * sizeof(*(_da).data)\
    );\
  --(_da).size;\
} while(0)

void darray_remove(void *darrayAny, usz index, usz typeSize);

#define darrayRemoveMany(_da, _index, _amount) do{\
  if(_index < (_da).size - _amount)\
    memmove(\
      (_da).data + (_index),\
      (_da).data + (_index) + (_amount),\
      ((_da).size - (_index) - (_amount)) * sizeof(*(_da).data)\
    );\
  (_da).size -= (_amount);\
} while(0)

void darray_removeMany(void *darrayAny, usz index, usz amount, usz typeSize);

#define darrayDestroy(_da) do{\
  free((_da).data);\
  memset(&(_da), 0, sizeof(_da));\
} while(0)

void darray_destroy(void *darrayAny);

#define darrayRecFree(_da) do{\
  for(usz i = 0; i < (_da).size; ++i)\
    free((_da).data[i]);\
} while(0)

void darray_recFree(void *darrayAny);

#define darrayIterate(_da, _type)\
  for(_type *_ref = NULL; !_ref; _ref = NULL+1)\
  for(_type _el; !_ref; _ref = NULL+1)\
  for(usz _i = 0; (_ref = (_da).data + _i, _el = *_ref, _i < (_da).size); ++_i)

#endif

