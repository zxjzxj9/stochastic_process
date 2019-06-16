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

const double invsqrt2 = 0.70710678118;

inline double cdf(double x) {
    return 0.5*(erf(invsqrt2*x)+1);
}

double vanilla_option::calc_call_price() {
    double sigma_sqrt_T = sigma*sqrt(T);
    double d1 = (log(S/K)+(r+0.5*sigma*sigma)*T)/sigma_sqrt_T;
    double d2 = (log(S/K)+(r+0.5*sigma*sigma)*T)/sigma_sqrt_T - sigma_sqrt_T;
    return S*cdf(d1) - K*exp(-r*T)*cdf(d2);
}

double vanilla_option::calc_put_price() {
    double sigma_sqrt_T = sigma*sqrt(T);
    double d1 = (log(S/K)+(r+0.5*sigma*sigma)*T)/sigma_sqrt_T;
    double d2 = (log(S/K)+(r+0.5*sigma*sigma)*T)/sigma_sqrt_T - sigma_sqrt_T;
    return -S*cdf(-d1) + K*exp(-r*T)*cdf(-d2) ;
}

template <typename func>
double vanilla_option::mc_asset(func f) {
    // Perform Monte Carlo simulation for assets, with function pointer passed
    const int sample_num = 1000000;
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::normal_distribution<double> dist(0, sigma*sqrt(T));
    double Sp = S*exp((r-0.5*sigma*sigma)*T);
    double sample_sum = 0.0;
    for(int i=0; i<sample_num; i++) {
        sample_sum += f(Sp*exp(sigma*T*dist(gen)));
    }
    sample_sum /= static_cast<double>(sample_num);
    sample_sum *= exp(-r*T);
    return sample_sum;
}

double vanilla_option::mc_call_price() {
    return mc_asset([this](double x){return std::max(x - K, 0.0);});
}

double vanilla_option::mc_put_price() {
    return mc_asset([this](double x){return std::max(K - x, 0.0);});
}
