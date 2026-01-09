#include "lowmat.h"

#include <assert.h>
#include <stdarg.h>

void matNullify(Mat mat){
  u32 ite = mat.r * mat.c;
  for(u32 i = 0; i < ite; ++i)
    mat.data[i] = 0.f;
}

void matDot(Mat fun, Mat arg, Mat out){
  assert(fun.c == arg.r);
  assert(fun.r == out.r);
  assert(arg.c == out.c);
  matNullify(out);
  float (*arrFun)[fun.r][fun.c] = (void*)fun.data;
  float (*arrArg)[fun.c][arg.c] = (void*)arg.data;
  float (*arrOut)[fun.r][arg.c] = (void*)out.data;
  for (u32 i = 0; i < fun.r; ++i)
    for (u32 k = 0; k < fun.c; ++k){
      float funCur = (*arrFun)[i][k];
      for (u32 j = 0; j < arg.c; ++j)
        (*arrOut)[i][j] += funCur * (*arrArg)[k][j];
    }
}

void matFillf(Mat mat, ...){
  va_list args;
  va_start(args, mat);
  u32 ite = mat.r * mat.c;
  for (u32 i = 0; i < ite; ++i)
    mat.data[i] = va_arg(args, double);
  va_end(args);
}

void matFill(Mat mat, ...){
  va_list args;
  va_start(args, mat);
  u32 ite = mat.r * mat.c;
  for (u32 i = 0; i < ite; ++i)
    mat.data[i] = va_arg(args, int);
  va_end(args);
}

void matRandomize(Mat mat){
  u32 ite = mat.r * mat.c;
  for (u32 i = 0; i < ite; ++i)
    mat.data[i] = (float)rand() / (float)RAND_MAX * 2.f - 1.f;
}

void matAdd(Mat dst, Mat val){
  assert(dst.r == val.r && dst.c == val.c);
  u32 ite = dst.r * dst.c;
  for (u32 i = 0; i < ite; ++i)
    dst.data[i] += val.data[i];
}

void subMat(Mat dst, Mat val){
  assert(dst.r == val.r && dst.c == val.c);
  u32 ite = dst.r * dst.c;
  for (u32 i = 0; i < ite; ++i)
    dst.data[i] -= val.data[i];
}

void matScale(Mat dst, float val){
  u32 ite = dst.r * dst.c;
  for (u32 i = 0; i < ite; ++i)
    dst.data[i] *= val;
}

void matCopy(Mat src, Mat dst){
  assert(src.r == dst.r && src.c == dst.c);
  u32 ite = dst.r * dst.c;
  for (u32 i = 0; i < ite; ++i)
    dst.data[i] = src.data[i];
}

void matTranspose(Mat src, Mat dst){
  assert(src.r == dst.c && src.c == dst.r);
  for (u32 i = 0; i < src.r; ++i)
  for (u32 j = 0; j < src.c; ++j)
    dst.data[j * dst.c + i] = src.data[i * src.c + j];
}

void matReduce(Mat src, Mat dst, unsigned x, unsigned y){
  u32 count = 0;
  for (u32 i = 0; i < src.r; i++) {
    if (i == y) continue;
    for (u32 j = 0; j < src.c; j++) {
      if (j == x) continue;
      dst.data[count] = src.data[i * src.c + j];
      count++;
    }
  }
}

float matDeterminant(Mat mat){
  assert(mat.r == mat.c);
  switch(mat.r){
  case 1:
    return mat.data[0];
  case 2:
    return mat.data[0] * mat.data[3] - mat.data[1] * mat.data[2];
  case 3:
    return
    mat.data[0] *
      (mat.data[4] * mat.data[8] - mat.data[5] * mat.data[7]) +
    mat.data[1] *
      (mat.data[5] * mat.data[6] - mat.data[3] * mat.data[8]) +
    mat.data[2] *
      (mat.data[3] * mat.data[7] - mat.data[4] * mat.data[6]);
  }

  float coSum = 0.f;
  for (unsigned i = 0; i < mat.r; i++) {
    Mat buffer = {mat.r - 1, mat.c - 1, 0};
    matAlloc(&buffer);
    matReduce(mat, buffer, i, 0);
    coSum += mat.data[i] * (i & 1 ? -1.f : 1.f) * matDeterminant(buffer);
    free(buffer.data);
  }
  return coSum;
}