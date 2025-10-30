#define _USE_MATH_DEFINES
#include <math.h>

struct Complex{
  union{
    struct{
      double a, b;
    };
    struct{
      double real, imaginary;
    };
  };
};
typedef struct Complex Complex;

static inline Complex Complex_add(Complex x, Complex y){
  return (Complex){.a = x.a + y.a, .b = x.b + y.b};
}
static inline Complex Complex_sub(Complex x, Complex y){
  return (Complex){.a = x.a - y.a, .b = x.b - y.b};
}
static inline Complex Complex_mul(Complex x, Complex y){
  return (Complex){.a = x.a * y.a - x.b * y.b, .b = x.a * y.b + x.b * y.a};
}
static inline Complex Complex_div(Complex x, Complex y){
  double divisor = y.a * y.a + y.b * y.b;
  return (Complex){.a = (x.a * y.a - x.b * y.b) / divisor, .b = (x.b * y.a - x.a * y.b) / divisor};
}
static Complex Complex_pow(Complex z, double n, int k){
  double ang = n * (atan2(z.b, z.a) + k * 2. * M_PI);
  double base = pow(hypot(z.a, z.b), n);
  return (Complex){ang == M_PI + M_PI_2 || ang == M_PI_2 ? 0.0 : base * cos(ang), ang == M_PI || ang == 0.0 ? 0.0 : base * sin(ang)};
}