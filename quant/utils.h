//
// Created by Victor Zhang on 2019/6/21.
//

#ifndef NUMERICAL_QUANT_PRACTICE_UTILS_H
#define NUMERICAL_QUANT_PRACTICE_UTILS_H

#include "math.h"

const double invsqrt2 = 0.70710678118;

inline double cdf(double x) {
    return 0.5*(erf(invsqrt2*x)+1);
}
#endif //NUMERICAL_QUANT_PRACTICE_UTILS_H
