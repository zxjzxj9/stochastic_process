#include "process.h"
#include <cmath>
#include <stdexcept>

// ============================================================
// WienerProcess
// ============================================================

double WienerProcess::simulate_terminal(double T, RNG& rng) const {
    return std::sqrt(T) * rng.normal();
}

std::vector<double> WienerProcess::simulate_path(double T, int n_steps, RNG& rng) const {
    if (n_steps <= 0) throw std::invalid_argument("n_steps must be positive");
    double sqrt_dt = std::sqrt(T / n_steps);
    std::vector<double> path(n_steps + 1);
    path[0] = 0.0;
    for (int i = 1; i <= n_steps; ++i)
        path[i] = path[i - 1] + sqrt_dt * rng.normal();
    return path;
}

std::vector<std::vector<double>> WienerProcess::simulate_paths(
    double T, int n_steps, int n_paths, RNG& rng) const
{
    std::vector<std::vector<double>> paths(n_paths);
    for (auto& p : paths)
        p = simulate_path(T, n_steps, rng);
    return paths;
}

// ============================================================
// GBMProcess
// ============================================================

GBMProcess::GBMProcess(double S0, double r, double sigma)
    : S0_(S0), r_(r), sigma_(sigma) {}

double GBMProcess::simulate_terminal(double T, RNG& rng) const {
    double drift     = (r_ - 0.5 * sigma_ * sigma_) * T;
    double diffusion = sigma_ * std::sqrt(T) * rng.normal();
    return S0_ * std::exp(drift + diffusion);
}

std::vector<double> GBMProcess::simulate_path(double T, int n_steps, RNG& rng) const {
    if (n_steps <= 0) throw std::invalid_argument("n_steps must be positive");
    double dt       = T / n_steps;
    double drift    = (r_ - 0.5 * sigma_ * sigma_) * dt;
    double sqrt_dt  = std::sqrt(dt);

    std::vector<double> path(n_steps + 1);
    path[0] = S0_;
    for (int i = 1; i <= n_steps; ++i)
        path[i] = path[i - 1] * std::exp(drift + sigma_ * sqrt_dt * rng.normal());
    return path;
}

std::vector<std::vector<double>> GBMProcess::simulate_paths(
    double T, int n_steps, int n_paths, RNG& rng) const
{
    std::vector<std::vector<double>> paths(n_paths);
    for (auto& p : paths)
        p = simulate_path(T, n_steps, rng);
    return paths;
}

// ============================================================
// MertonProcess
// ============================================================

MertonProcess::MertonProcess(double S0, double r, double sigma,
                             double lam, double mu_J, double sigma_J)
    : S0_(S0), r_(r), sigma_(sigma), lam_(lam), mu_J_(mu_J), sigma_J_(sigma_J) {}

double MertonProcess::compensator() const {
    return std::exp(mu_J_ + 0.5 * sigma_J_ * sigma_J_) - 1.0;
}

double MertonProcess::simulate_terminal(double T, RNG& rng) const {
    double kbar      = compensator();
    double drift     = (r_ - lam_ * kbar - 0.5 * sigma_ * sigma_) * T;
    double diffusion = sigma_ * std::sqrt(T) * rng.normal();

    // Draw total number of jumps over [0,T]
    int n_jumps = rng.poisson(lam_ * T);
    double jump_sum = 0.0;
    for (int i = 0; i < n_jumps; ++i)
        jump_sum += rng.normal(mu_J_, sigma_J_);

    return S0_ * std::exp(drift + diffusion + jump_sum);
}

std::vector<double> MertonProcess::simulate_path(double T, int n_steps, RNG& rng) const {
    if (n_steps <= 0) throw std::invalid_argument("n_steps must be positive");
    double dt       = T / n_steps;
    double kbar     = compensator();
    double drift    = (r_ - lam_ * kbar - 0.5 * sigma_ * sigma_) * dt;
    double sqrt_dt  = std::sqrt(dt);

    std::vector<double> path(n_steps + 1);
    path[0] = S0_;
    for (int i = 1; i <= n_steps; ++i) {
        double diffusion = sigma_ * sqrt_dt * rng.normal();
        int n_jumps = rng.poisson(lam_ * dt);
        double jump_sum = 0.0;
        for (int j = 0; j < n_jumps; ++j)
            jump_sum += rng.normal(mu_J_, sigma_J_);
        path[i] = path[i - 1] * std::exp(drift + diffusion + jump_sum);
    }
    return path;
}

std::vector<std::vector<double>> MertonProcess::simulate_paths(
    double T, int n_steps, int n_paths, RNG& rng) const
{
    std::vector<std::vector<double>> paths(n_paths);
    for (auto& p : paths)
        p = simulate_path(T, n_steps, rng);
    return paths;
}
