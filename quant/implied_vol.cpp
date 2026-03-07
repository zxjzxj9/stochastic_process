//
// Implied Volatility solver using Newton-Raphson
//

#include "implied_vol.h"
#include "utils.h"
#include <cmath>
#include <stdexcept>

// Compute BS price and vega for a given sigma, avoiding repeated code
static void bs_price_and_vega(double S, double K, double r, double T, double sigma,
                              bool is_call, double& price, double& vega_val) {
    double sqrt_T = sqrt(T);
    double sigma_sqrt_T = sigma * sqrt_T;
    double d1 = (log(S / K) + (r + 0.5 * sigma * sigma) * T) / sigma_sqrt_T;
    double d2 = d1 - sigma_sqrt_T;

    if (is_call) {
        price = S * cdf(d1) - K * exp(-r * T) * cdf(d2);
    } else {
        price = -S * cdf(-d1) + K * exp(-r * T) * cdf(-d2);
    }

    // Vega is the same for calls and puts
    vega_val = S * normal(d1) * sqrt_T;
}

double implied_vol(double market_price, double S, double K, double r, double T,
                   bool is_call, double tol, int max_iter) {
    // Initial guess: start at 0.3 (typical market vol)
    double sigma = 0.3;

    for (int i = 0; i < max_iter; i++) {
        double bs_price, vega_val;
        bs_price_and_vega(S, K, r, T, sigma, is_call, bs_price, vega_val);

        double diff = bs_price - market_price;

        if (std::abs(diff) < tol) {
            return sigma;
        }

        if (vega_val < 1e-12) {
            throw std::runtime_error("Vega near zero — cannot converge (option may be deep ITM/OTM)");
        }

        sigma -= diff / vega_val;

        // Clamp sigma to a reasonable range
        if (sigma < 1e-6) sigma = 1e-6;
        if (sigma > 10.0) sigma = 10.0;
    }

    throw std::runtime_error("Implied vol did not converge within max iterations");
}
