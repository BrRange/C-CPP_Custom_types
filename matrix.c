#include "matrix.h"
#include <string.h>

void matNullify(Mat mat){
  memset(mat.data, 0, 4ull * mat.r * mat.c);
}

void matDot(Mat fun, Mat arg, Mat out){
  matNullify(out);
  for(u32 i = 0; i < fun.r; ++i)
    for(u32 k = 0; k < fun.c; ++k){
      float funCur = fun.data[i * fun.c + k];
      for(u32 j = 0; j < arg.c; ++j)
        out.data[i * arg.c + j] += funCur * arg.data[k * arg.c + j];
    }
}

void matFillf(Mat mat, f32 *arr, u32 len){
  memcpy(mat.data, arr, len * sizeof(f32));
}

void matFill(Mat mat, i32 *arr, u32 len){
  for(u32 i = 0; i < len; ++i)
    mat.data[i] = arr[i];
}

void matRandomize(Mat mat){
  u32 ite = mat.r * mat.c;
  for(u32 i = 0; i < ite; ++i)
    mat.data[i] = (float)rand() / (float)RAND_MAX * 2.f - 1.f;
}

void matAdd(Mat dst, Mat val){
  u32 ite = dst.r * dst.c;
  for(u32 i = 0; i < ite; ++i)
    dst.data[i] += val.data[i];
}

void subMat(Mat dst, Mat val){
  u32 ite = dst.r * dst.c;
  for(u32 i = 0; i < ite; ++i)
    dst.data[i] -= val.data[i];
}

void matScale(Mat dst, float val){
  u32 ite = dst.r * dst.c;
  for(u32 i = 0; i < ite; ++i)
    dst.data[i] *= val;
}

void matCopy(Mat dest, Mat src){
  u32 ite = dest.r * dest.c;
  memcpy(dest.data, src.data, ite * sizeof(f32));
}

void matTranspose(Mat src, Mat dst){
  for(u32 i = 0; i < src.r; ++i)
  for(u32 j = 0; j < src.c; ++j)
    dst.data[j * dst.c + i] = src.data[i * src.c + j];
}

void matReduce(Mat src, Mat dst, u32 x, u32 y){
  u32 count = 0;
  for(u32 i = 0; i < src.r; i++){
    if (i == y) continue;
    for(u32 j = 0; j < src.c; j++){
      if (j == x) continue;
      dst.data[count] = src.data[i * src.c + j];
      count++;
    }
  }
}

float matDeterminant(Mat mat){
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
  for(unsigned i = 0; i < mat.r; i++){
    Mat buffer = {mat.r - 1, mat.c - 1, 0};
    matAlloc(&buffer);
    matReduce(mat, buffer, i, 0);
    coSum += mat.data[i] * (i & 1 ? -1.f : 1.f) * matDeterminant(buffer);
    free(buffer.data);
  }
  return coSum;
}