//
// Created by Victor Zhang on 2019/6/8.
//

#include <iostream>
#include "vanilla_option.h"
#include "greeks.h"
#include "implied_vol.h"

int main() {
    // K=100, r=0.05, T=1.0, S=100, sigma=0.2
    vanilla_option option(100, 0.05, 1.0, 100, 0.2);
    std::cout << "=== Vanilla Option Pricing ===" << std::endl;
    std::cout << "Analytic call price: " << option.calc_call_price()
              << " | Analytic put price: " << option.calc_put_price() << std::endl;
    std::cout << "MC call price: " << option.mc_call_price()
              << " | MC put price: " << option.mc_put_price() << std::endl;

    std::cout << std::endl << "=== Greeks ===" << std::endl;
    delta d(100, 0.05, 1.0, 100, 0.2);
    std::cout << "Delta call: " << d.call_price()
              << " | Delta put: " << d.put_price() << std::endl;

    gamma g(100, 0.05, 1.0, 100, 0.2);
    std::cout << "Gamma call: " << g.call_price()
              << " | Gamma put: " << g.put_price() << std::endl;

    vega v(100, 0.05, 1.0, 100, 0.2);
    std::cout << "Vega call: " << v.call_price()
              << " | Vega put: " << v.put_price() << std::endl;

    theta t(100, 0.05, 1.0, 100, 0.2);
    std::cout << "Theta call: " << t.call_price()
              << " | Theta put: " << t.put_price() << std::endl;

    // Implied Volatility: recover sigma from BS prices
    std::cout << std::endl << "=== Implied Volatility ===" << std::endl;
    double call_price = option.calc_call_price();  // 10.4506 (with sigma=0.2)
    double put_price = option.calc_put_price();     // 5.57353

    double iv_call = implied_vol(call_price, 100, 100, 0.05, 1.0, true);
    double iv_put  = implied_vol(put_price,  100, 100, 0.05, 1.0, false);
    std::cout << "IV from call price (" << call_price << "): " << iv_call
              << " (expected: 0.2)" << std::endl;
    std::cout << "IV from put price  (" << put_price << "): " << iv_put
              << " (expected: 0.2)" << std::endl;

    // Test with different market prices
    double iv1 = implied_vol(15.0, 100, 100, 0.05, 1.0, true);
    double iv2 = implied_vol(2.0,  100, 100, 0.05, 1.0, false);
    std::cout << "IV from call price (15.0): " << iv1 << std::endl;
    std::cout << "IV from put price  (2.0):  " << iv2 << std::endl;

    return 0;
}
