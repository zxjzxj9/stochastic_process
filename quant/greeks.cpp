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
    double d1 = (log(S/K) + (r + sigma*sigma/2)*T) / (sigma*sqrt(T));
    return cdf(d1);
}

double delta::put_price() {
    double d1 = (log(S/K) + (r + sigma*sigma/2)*T) / (sigma*sqrt(T));
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
    double d1 = (log(S/K) + (r + sigma*sigma/2)*T) / (sigma*sqrt(T));
    return normal(d1) / (S*sigma*sqrt(T));
}

double gamma::put_price() {
    // gamma is the same for calls and puts
    double d1 = (log(S/K) + (r + sigma*sigma/2)*T) / (sigma*sqrt(T));
    return normal(d1) / (S*sigma*sqrt(T));
}

double gamma::mc_call_price() {
    return 0;
}

double gamma::mc_put_price() {
    return 0;
}

// vega sensitivity
vega::vega(double K, double r, double T, double S, double sigma):
        greeks(K, r, T, S, sigma) {
}

double vega::call_price() {
    // vega is the same for calls and puts
    double d1 = (log(S/K) + (r + sigma*sigma/2)*T) / (sigma*sqrt(T));
    return S * normal(d1) * sqrt(T);
}

double vega::put_price() {
    double d1 = (log(S/K) + (r + sigma*sigma/2)*T) / (sigma*sqrt(T));
    return S * normal(d1) * sqrt(T);
}

double vega::mc_call_price() {
    return 0;
}

double vega::mc_put_price() {
    return 0;
}

// theta sensitivity
theta::theta(double K, double r, double T, double S, double sigma):
        greeks(K, r, T, S, sigma) {
}

double theta::call_price() {
    double d1 = (log(S/K) + (r + sigma*sigma/2)*T) / (sigma*sqrt(T));
    double d2 = d1 - sigma*sqrt(T);
    return -S*sigma*normal(d1) / (2*sqrt(T)) - r*K*exp(-r*T)*cdf(d2);
}

double theta::put_price() {
    double d1 = (log(S/K) + (r + sigma*sigma/2)*T) / (sigma*sqrt(T));
    double d2 = d1 - sigma*sqrt(T);
    return -S*sigma*normal(d1) / (2*sqrt(T)) + r*K*exp(-r*T)*cdf(-d2);
}

double theta::mc_call_price() {
    return 0;
}

double theta::mc_put_price() {
    return 0;
}
