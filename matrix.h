#ifndef MATRIX_H_
#define MATRIX_H_

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
  f32 *data;
  u32 r, c;
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
  mat->data = malloc(sizeof(f32) * mat->r * mat->c);
}

static inline void matFree(Mat mat){
  free(mat.data);
}

void matFillf(Mat mat, f32 *arr, u32 len);

void matFill(Mat mat, i32 *arr, u32 len);

void matRandomize(Mat mat);

void matAdd(Mat dest, Mat val);

void subMat(Mat dest, Mat val);

void matScale(Mat dest, float val);

void matCopy(Mat dest, Mat src);

void matTranspose(Mat dest, Mat src);

void matReduce(Mat dest, Mat src, u32 x, u32 y);

float matDeterminant(Mat mat);

#endif