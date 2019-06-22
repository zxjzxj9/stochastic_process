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

double gamma::call_price() {
    double d1 = log(S/K) + (r+sigma*sigma/2)*T/(sigma*sqrt(T));
    return exp(-r*T)*normal(d1)/(S*sigma*sqrt(T));
}

double gamma::put_price() {
    double d2 = log(S/K) + (r+sigma*sigma/2)*T/(sigma*sqrt(T)) - sigma*sqrt(T);
    return -exp(-r*T)*normal(-d2)/(S*sigma*sqrt(T));
}

double gamma::mc_call_price() {
    return 0;
}

double gamma::mc_put_price() {
    return 0;
}
