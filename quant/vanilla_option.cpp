//
// Created by Victor Zhang on 2019/6/7.
//

#include "vanilla_option.h"

vanilla_option& vanilla_option::vanilla_option(const vanilla_option v) {
    K = v.K;
    r = v.r;
    T = v.T;
    S = v.S;
    sigma = v.sigma;
    return *this;
}

vanilla_option::vanilla_option(const vanilla_option v) {
    K = v.K;
    r = v.r;
    T = v.T;
    S = v.S;
    sigma = v.sigma;
}