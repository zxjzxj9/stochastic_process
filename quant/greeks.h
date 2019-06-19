//
// Created by Victor Zhang on 2019/6/18.
//

#ifndef NUMERICAL_QUANT_PRACTICE_GREEKS_H
#define NUMERICAL_QUANT_PRACTICE_GREEKS_H


class greeks {

public:
    greeks(double K, double r, double T, double S, double sigma)
            : K(K), r(r), T(T), S(S), sigma(sigma) {
    }

    greeks();

    virtual double call_price();
    virtual double put_price();
    virtual double mc_call_price();
    virtual double mc_put_price();
private:
    double K, r, T, S, sigma;
};

class delta: public greeks {

};

#endif //NUMERICAL_QUANT_PRACTICE_GREEKS_H
