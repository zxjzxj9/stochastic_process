//
// Created by Victor Zhang on 2019/6/7.
//

#ifndef NUMERICAL_QUANT_PRACTICE_VANILLA_OPTION_H
#define NUMERICAL_QUANT_PRACTICE_VANILLA_OPTION_H

#include <cmath>
#include <random>
#include <algorithm>
#include <functional>

// Use Black-Scholes model to compute option put/call price
class vanilla_option {
public:
    /*
     * Strike price: K
     * Interest rate: r
     * Option time: T
     * Underlying asset price: S
     * Variance of asset price: sigma
     */
    vanilla_option(double K, double r, double T, double S, double sigma)
            : K(K), r(r), T(T), S(S), sigma(sigma) {

    }

    vanilla_option(){}

    // copy constructor
    vanilla_option(const vanilla_option& v);
    vanilla_option& operator=(const vanilla_option& v);
    virtual ~vanilla_option(){}

    double calc_put_price();
    double calc_call_price();

    template <typename func>
    double mc_asset(func);

    double mc_put_price();
    double mc_call_price();


private:
    double K, r, T, S, sigma;
};


#endif //NUMERICAL_QUANT_PRACTICE_VANILLA_OPTION_H
