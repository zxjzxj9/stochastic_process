#pragma once

#include "rng.h"
#include <vector>

/**
 * Standard Wiener process (Brownian motion).
 * Paths start at 0 and have increments ~ N(0, dt).
 */
class WienerProcess {
public:
    // Single terminal value B(T)
    double simulate_terminal(double T, RNG& rng) const;

    // Full path: returns (n_steps+1) values from t=0 to t=T
    std::vector<double> simulate_path(double T, int n_steps, RNG& rng) const;

    // n_paths independent paths, each of length (n_steps+1)
    std::vector<std::vector<double>> simulate_paths(double T, int n_steps, int n_paths, RNG& rng) const;

    // Required by mc_price template; Wiener process has r=0
    double r() const { return 0.0; }
};

/**
 * Geometric Brownian Motion under the risk-neutral measure:
 *   dS = r*S*dt + sigma*S*dW
 *
 * Exact discretization (no Euler error):
 *   S(t+dt) = S(t) * exp((r - sigma^2/2)*dt + sigma*sqrt(dt)*Z)
 */
class GBMProcess {
public:
    GBMProcess(double S0, double r, double sigma);

    // Terminal value S(T)
    double simulate_terminal(double T, RNG& rng) const;

    // Full path: (n_steps+1) prices from t=0 to t=T
    std::vector<double> simulate_path(double T, int n_steps, RNG& rng) const;

    // n_paths independent paths
    std::vector<std::vector<double>> simulate_paths(double T, int n_steps, int n_paths, RNG& rng) const;

    double get_S0()    const { return S0_;    }
    double r()         const { return r_;     }
    double get_sigma() const { return sigma_; }

private:
    double S0_, r_, sigma_;
};

/**
 * Merton (1976) jump-diffusion model under the risk-neutral measure:
 *   dS/S = (r - lambda*kbar)*dt + sigma*dW + (e^J - 1)*dN
 *
 * where:
 *   N ~ Poisson(lambda) is the jump count process
 *   J ~ N(mu_J, sigma_J^2) is the log-jump size
 *   kbar = E[e^J - 1] = exp(mu_J + 0.5*sigma_J^2) - 1  (compensator)
 */
class MertonProcess {
public:
    MertonProcess(double S0, double r, double sigma,
                  double lam, double mu_J, double sigma_J);

    // kbar = exp(mu_J + 0.5*sigma_J^2) - 1
    double compensator() const;

    // Terminal value via exact simulation (no time-stepping error)
    double simulate_terminal(double T, RNG& rng) const;

    // Full path via Euler steps with Poisson jumps at each step
    std::vector<double> simulate_path(double T, int n_steps, RNG& rng) const;

    // n_paths independent paths
    std::vector<std::vector<double>> simulate_paths(double T, int n_steps, int n_paths, RNG& rng) const;

    double get_S0()      const { return S0_;      }
    double r()           const { return r_;       }
    double get_sigma()   const { return sigma_;   }
    double get_lam()     const { return lam_;     }
    double get_mu_J()    const { return mu_J_;    }
    double get_sigma_J() const { return sigma_J_; }

private:
    double S0_, r_, sigma_, lam_, mu_J_, sigma_J_;
};
