//
// Created by Victor Zhang on 2019/6/21.
//

#ifndef NUMERICAL_QUANT_PRACTICE_UTILS_H
#define NUMERICAL_QUANT_PRACTICE_UTILS_H

#include <cmath>

const double invsqrt2 = 0.70710678118;
const double invsqrt2pi = 0.5 * M_2_SQRTPI * M_SQRT1_2;

inline double cdf(double x) {
    return 0.5 * (erf(invsqrt2 * x) + 1);
}

inline double normal(double x) {
    return invsqrt2pi * exp(-x * x / 2.0);
}
#endif //NUMERICAL_QUANT_PRACTICE_UTILS_H
