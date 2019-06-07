//
// Created by Victor Zhang on 2019/6/7.
//

#ifndef NUMERICAL_QUANT_PRACTICE_VANILLA_OPTION_H
#define NUMERICAL_QUANT_PRACTICE_VANILLA_OPTION_H

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
    vanilla_option& operator=(const vanilla_option& v) {
        K = v.K;
        r = v.r;
        T = v.T;
        S = v.S;
        sigma = v.sigma;
        return *this;
    }

private:
    double K, r, T, S, sigma;
};


#endif //NUMERICAL_QUANT_PRACTICE_VANILLA_OPTION_H
