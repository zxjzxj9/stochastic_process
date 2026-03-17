#pragma once

#include <random>
#include <vector>
#include <cmath>
#include <cstdint>

/**
 * Random Number Generator wrapper around std::mt19937_64.
 * Provides convenience methods for distributions used in stochastic simulation.
 */
class RNG {
public:
    explicit RNG(unsigned seed = std::random_device{}())
        : engine_(seed), normal_dist_(0.0, 1.0) {}

    void seed(unsigned s) {
        engine_.seed(s);
        normal_dist_.reset();
    }

    // Standard normal N(0,1)
    double normal() { return normal_dist_(engine_); }

    // General normal N(mean, stddev^2)
    double normal(double mean, double stddev) {
        return mean + stddev * normal_dist_(engine_);
    }

    // Poisson(lambda) — returns number of events
    int poisson(double lambda) {
        std::poisson_distribution<int> dist(lambda);
        return dist(engine_);
    }

    // Uniform [0, 1)
    double uniform() {
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(engine_);
    }

    // Vector of n iid standard normals
    std::vector<double> normal_vector(size_t n) {
        std::vector<double> v(n);
        for (auto& x : v) x = normal_dist_(engine_);
        return v;
    }

private:
    std::mt19937_64 engine_;
    std::normal_distribution<double> normal_dist_;
};
