//
// Created by Victor Zhang on 2019/6/8.
//

#include <iostream>
#include "vanilla_option.h"
#include "greeks.h"

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

    return 0;
}
