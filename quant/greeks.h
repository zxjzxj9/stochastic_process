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

    virtual double call_price() = 0;
    virtual double put_price() = 0;
    virtual double mc_call_price() = 0;
    virtual double mc_put_price() = 0;

protected:
    double K, r, T, S, sigma;
};

// delta sensitivity
class delta: public greeks {
public:
    delta(double K, double r, double T, double S, double sigma);

    virtual double call_price();
    virtual double put_price();
    virtual double mc_call_price();
    virtual double mc_put_price();
//private:
//    double K, r, T, S, sigma;
};

// gamma sensitivity
class gamma: public greeks {
public:
    gamma(double K, double r, double T, double S, double sigma);

    virtual double call_price();
    virtual double put_price();
    virtual double mc_call_price();
    virtual double mc_put_price();
//private:
//    double K, r, T, S, sigma;
};

#endif //NUMERICAL_QUANT_PRACTICE_GREEKS_H

// vega sensitivity
class vega: public greeks {
public:
    vega(double K, double r, double T, double S, double sigma);

    virtual double call_price();
    virtual double put_price();
    virtual double mc_call_price();
    virtual double mc_put_price();

};