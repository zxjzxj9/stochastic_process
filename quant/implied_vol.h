//
// Implied Volatility solver using Newton-Raphson
//

#ifndef NUMERICAL_QUANT_PRACTICE_IMPLIED_VOL_H
#define NUMERICAL_QUANT_PRACTICE_IMPLIED_VOL_H

#include <stdexcept>

// Solve for implied volatility given a market option price.
// market_price: observed option price
// S: underlying asset price
// K: strike price
// r: risk-free interest rate
// T: time to expiration
// is_call: true for call, false for put
// tol: convergence tolerance
// max_iter: maximum Newton-Raphson iterations
// Returns the implied volatility sigma
double implied_vol(double market_price, double S, double K, double r, double T,
                   bool is_call, double tol = 1e-8, int max_iter = 100);

#endif //NUMERICAL_QUANT_PRACTICE_IMPLIED_VOL_H
