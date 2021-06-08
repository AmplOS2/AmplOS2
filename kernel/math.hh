#pragma once
#include <stdint.h>

// TODO: make portable
double trunc(double x) {
        double y;
        asm volatile("frintz %d0, %d1" : "=w"(y) : "w"(x));
        return y;
}

double fmod(double x, double y) { return x - trunc(x / y) * y; }

// this is a custom sine using the taylor formula (https://en.wikipedia.org/wiki/Taylor_series)
// TODO: on x86 `fsin` is a thing
// TODO: check out llvm-project/libc/src/math/generic/sinf.cpp
constexpr double sin(double x) {
        double x2 = x * x;
        double x4 = x2 * x2;
        double t1 = x * (1.0 - x2 / (2 * 3));

        double x5 = x * x4;
        double t2 = x5 * (1.0 - x2 / (6 * 7)) / (1.0 * 2 * 3 * 4 * 5);

        double x9 = x5 * x4;
        double t3 = x9 * (1.0 - x2 / (10 * 11)) / (1.0 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9);

        double x13 = x9 * x4;
        double t4  = x13 * (1.0 - x2 / (14 * 15)) /
                    (1.0 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10 * 11 * 12 * 13);

        return t4 + t3 + t2 + t1;
}
