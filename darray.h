#ifndef DARRAYH
#define DARRAYH

/*
Expected format:
{
    type *data;
    u64 size, cap;
}

Expected new data:
{NULL, 0, 0}
*/

#include <stdlib.h>
#include <string.h>
#include "rustydef.h"

void *safeRealloc(void *ptr, int *size, int targ){
  void *newPtr = realloc(ptr, targ);
  if(newPtr){
    *size = targ;
    return newPtr;
  }
  return ptr;
}

#define darrayGrow(da) do{\
  i64 old = (da).cap;\
  while((da).size > (da).cap){\
    if(!(da).cap){\
      (da).cap = 2ull;\
    } else{\
      (da).cap += (da).cap < 8 ? (da).cap : 8ull;\
    }\
  }\
  (da).data = realloc((da).data, sizeof(*(da).data) * (da).cap);\
  printf("Resize: %2zu -> %zu\n", old, (da).cap);\
} while(0)

#define darrayShrink(da) do{\
  u64 old = (da).cap;\
  u64 newCap = (da).cap > 8 ? (da).cap - 8ull : (da).cap >> 1ull;\
  while(newCap > (da).size){\
    (da).cap = newCap;\
    newCap = (da).cap > 8 ? (da).cap - 8ull : (da).cap >> 1ull;\
  }\
  (da).data = realloc((da).data, darrayAllocSize(da));\
  printf("Resize: %2zu -> %zu\n", old, (da).cap);\
} while(0)

#define darrayUsedSize(da) ((da).size * sizeof(*(da).data))
#define darrayAllocSize(da) ((da).cap * sizeof(*(da).data))

#define darrayAppend(da, el) do{\
  ++(da).size;\
  if((da).size > (da).cap) darrayGrow(da);\
  (da).data[(da).size] = (el);\
} while(0)

#define darrayAppendMany(da, li, n) do{\
  (da).size += (n);\
  darrayGrow(da);\
  memcpy((da).data + (da).size - n, li, n * sizeof(*(da).data));\
} while(0)

#define darrayRemove(da, index) do{\
  if(index < (da).size - 1)\
    memcpy((da).data + (index), (da).data + (index) + 1, ((da).size - (index) - 1) * sizeof(*(da).data));\
  --(da).size;\
} while(0)

#define darrayDestroy(da) do{\
  free((da).data);\
  memset(&(da), 0, sizeof(da));\
} while(0)

#endif