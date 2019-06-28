//
// Created by Victor Zhang on 2019/6/18.
//

#include "greeks.h"
#include "utils.h"


// delta sensitivity
delta::delta(double K, double r, double T, double S, double sigma) :
        greeks(K, r, T, S, sigma) {
}

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

// gamma sensitivity
gamma::gamma(double K, double r, double T, double S, double sigma):
        greeks(K, r, T, S, sigma) {
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

// vega sensitvity
vega::vega(double K, double r, double T, double S, double sigma):
        greeks(K, r, T, S, sigma) {
}

double vega::call_price() {
    double d1 = log(S/K) + (r+sigma*sigma/2)*T/(sigma*sqrt(T));
    return S*exp(-r*T)*cdf(d1)*sqrt(T);
}

double vega::put_price() {
    double d2 = log(S/K) + (r+sigma*sigma/2)*T/(sigma*sqrt(T)) - sigma*sqrt(T);
    return K*exp(-r*T)*cdf(d2)*sqrt(T);
}

double vega::mc_call_price() {
    return 0;
}

double vega::mc_put_price() {
    return 0;
}

