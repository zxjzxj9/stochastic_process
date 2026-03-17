#pragma once

#include "rng.h"
#include "payoff.h"
#include <cmath>
#include <vector>

/**
 * Result of a Monte Carlo pricing run.
 */
struct MCResult {
    double price;      // discounted expected payoff
    double std_error;  // standard error of the estimate
    int    n_paths;    // number of simulated paths
};

/**
 * Monte Carlo pricer — works with any process type that provides:
 *   - double r() const
 *   - std::vector<double> simulate_path(double T, int n_steps, RNG&) const
 *
 * Template is header-only so it can be instantiated for any process.
 *
 * @param process   Process object (GBMProcess, MertonProcess, etc.)
 * @param payoff    Payoff functor deriving from Payoff
 * @param T         Maturity (years)
 * @param n_steps   Time-steps per path (use 1 for European terminal-only)
 * @param n_paths   Number of Monte Carlo paths
 * @param seed      RNG seed for reproducibility
 */
template <typename Process>
MCResult mc_price(const Process& process,
                  const Payoff&  payoff,
                  double T,
                  int    n_steps,
                  int    n_paths,
                  unsigned seed = 42)
{
    RNG rng(seed);
    double discount = std::exp(-process.r() * T);

    double sum    = 0.0;
    double sum_sq = 0.0;

    for (int i = 0; i < n_paths; ++i) {
        auto   path = process.simulate_path(T, n_steps, rng);
        double pv   = discount * payoff.evaluate(path);
        sum    += pv;
        sum_sq += pv * pv;
    }

    double mean     = sum / n_paths;
    double variance = (sum_sq / n_paths) - mean * mean;
    double std_err  = std::sqrt(std::max(variance, 0.0) / n_paths);

    return {mean, std_err, n_paths};
}
