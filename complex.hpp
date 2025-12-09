#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

struct Complex{
    double a, b;
    inline constexpr Complex(double r, double i = 0.0) : a(r), b(i) {}
    constexpr Complex power(const double n, const int k = 0) const{
        double ang = n * (atan2f(b, a) + k * 6.283185307179586);
        double base = pow(hypot(a, b), n);
        return Complex(ang == 4.71238898038469 || ang == M_PI_2 ? 0.0 : base * cos(ang), ang == M_PI || ang == 0.0 ? 0.0 : base * sin(ang));
    }
    inline constexpr Complex root(int n, int k = 0) const{
        return power(1.0 / n, k);
    }
    inline constexpr Complex operator+(const Complex &z) const{
        return Complex(a + z.a, b + z.b);
    }
    inline constexpr Complex operator-() const{
        return Complex(-a, -b);
    }
    inline constexpr Complex operator-(const Complex &z) const{
        return this->operator+(z.operator-());
    }
    inline constexpr Complex operator*(const Complex &z) const{
        return Complex(a * z.a - b * z.b, a * z.b + b * z.a);
    }
    inline constexpr Complex operator/(const Complex &z) const{
        double divisor = z.a * z.a + z.b * z.b;
        return Complex((a * z.a + b * z.b) / divisor, (b * z.a - z.b * a) / divisor);
    }
};