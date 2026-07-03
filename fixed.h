#ifndef FIXED_POINT_NUMBER_H_
#define FIXED_POINT_NUMBER_H_

#include "defs.h"

#define x8_lit(_i, _f) ((x8){.f = 16 * (_f), .i = (_i)})

typedef struct FixedByte{
  u8 f : 4, i : 4;
} FixedByte, x8;

typedef struct Fixed{
  u32 i : 16, f : 16;
} Fixed, x32;

void x8_display(x8 fixed, const char *end);

x8 x8_add(x8 a, x8 b);

x8 x8_sub(x8 a, x8 b);

x8 x8_mul(x8 a, x8 b);

u8 x8_imul(x8 a, u8 b) asm("x8_mul");

#endif