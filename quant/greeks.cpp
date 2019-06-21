//
// Created by Victor Zhang on 2019/6/18.
//

#include "greeks.h"
#include "utils.h"

double delta::call_price() {
    double d1 = log(S/K) + (r+sigma*sigma/2)*T/(sigma*sqrt(T));
    return cdf(d1);
}

double delta::put_price() {
    double d1 = log(S/K) + (r+sigma*sigma/2)*T/(sigma*sqrt(T));
    return cdf(d1) - 1;
}

double delta::mc_call_price() {
    return 0;
}

double delta::mc_put_price() {
    return 0;
}
