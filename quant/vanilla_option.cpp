//
// Created by Victor Zhang on 2019/6/7.
//

#include "vanilla_option.h"

vanilla_option& vanilla_option::operator=(const vanilla_option &v) {
    K = v.K;
    r = v.r;
    T = v.T;
    S = v.S;
    sigma = v.sigma;
    return *this;
}

vanilla_option::vanilla_option(const vanilla_option& v) {
    K = v.K;
    r = v.r;
    T = v.T;
    S = v.S;
    sigma = v.sigma;
}

double vanilla_option::calc_call_price() {
    double sigma_sqrt_T = sigma*sqrt(T);
    double d1 = (log(S/K)+(r+0.5*sigma*sigma)*T)/sigma_sqrt_T;
    double d2 = (log(S/K)+(r+0.5*sigma*sigma)*T)/sigma_sqrt_T - sigma_sqrt_T;
    return S*erf(d1) - K*exp(-r*T)*erf(d2);
}

double vanilla_option::calc_put_price() {
    double sigma_sqrt_T = sigma*sqrt(T);
    double d1 = (log(S/K)+(r+0.5*sigma*sigma)*T)/sigma_sqrt_T;
    double d2 = (log(S/K)+(r+0.5*sigma*sigma)*T)/sigma_sqrt_T - sigma_sqrt_T;
    return K*exp(-r*T)*erf(-d2) - S*erf(-d1);
}