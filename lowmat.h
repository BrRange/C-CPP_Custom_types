#ifndef MATRIXH
#define MATRIXH

#include <stdlib.h>
#include "rustydef.h"

/**
 * All memory allocation is done externally
 */

#define Matrix_expr(_mat, _expr_ij...)     \
  for (unsigned i = 0; i < (_mat).r; i++)  \
  for (unsigned j = 0; j < (_mat).c; j++)  \
  (_mat).data[i * (_mat).c + j] = (_expr_ij)

struct Mat {
  unsigned r, c;
  float *data;
};
typedef struct Mat Mat;

static inline float matRead(Mat mat, u32 x, u32 y){
  return mat.data[x * mat.c + y];
}

static inline void matWrite(Mat mat, u32 x, u32 y, float val){
  mat.data[x * mat.c + y] = val;
}

static inline float *matView(Mat mat, u32 x, u32 y){
  return &mat.data[x * mat.c + y];
}

void matNullify(Mat mat);

void matDot(Mat fun, Mat arg, Mat out);

static inline void matAlloc(Mat *mat){
  mat->data = malloc(sizeof(float) * mat->r * mat->c);
}

static inline void matFree(Mat mat){
  free(mat.data);
}

void matFillf(Mat mat, ...);

void matFill(Mat mat, ...);

void matRandomize(Mat mat);

void matAdd(Mat dst, Mat val);

void subMat(Mat dst, Mat val);

void matScale(Mat dst, float val);

void matCopy(Mat src, Mat dst);

void matTranspose(Mat src, Mat dst);

void matReduce(Mat src, Mat dst, unsigned x, unsigned y);

float matDeterminant(Mat mat);

#endif