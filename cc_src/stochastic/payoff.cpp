#include "payoff.h"
#include <cmath>

// ---- European ----

double EuropeanCall::evaluate(const std::vector<double>& path) const {
    return std::max(path.back() - K_, 0.0);
}

double EuropeanPut::evaluate(const std::vector<double>& path) const {
    return std::max(K_ - path.back(), 0.0);
}

double DigitalCall::evaluate(const std::vector<double>& path) const {
    return path.back() > K_ ? 1.0 : 0.0;
}

// ---- Asian ----

double AsianCall::evaluate(const std::vector<double>& path) const {
    double avg = std::accumulate(path.begin(), path.end(), 0.0) / static_cast<double>(path.size());
    return std::max(avg - K_, 0.0);
}

double AsianPut::evaluate(const std::vector<double>& path) const {
    double avg = std::accumulate(path.begin(), path.end(), 0.0) / static_cast<double>(path.size());
    return std::max(K_ - avg, 0.0);
}

// ---- Barrier ----

double UpAndOutCall::evaluate(const std::vector<double>& path) const {
    double max_price = *std::max_element(path.begin(), path.end());
    if (max_price >= barrier_) return 0.0;
    return std::max(path.back() - K_, 0.0);
}

double DownAndOutPut::evaluate(const std::vector<double>& path) const {
    double min_price = *std::min_element(path.begin(), path.end());
    if (min_price <= barrier_) return 0.0;
    return std::max(K_ - path.back(), 0.0);
}

// ---- Lookback ----

double FloatingLookbackCall::evaluate(const std::vector<double>& path) const {
    double min_price = *std::min_element(path.begin(), path.end());
    return path.back() - min_price;
}

double FloatingLookbackPut::evaluate(const std::vector<double>& path) const {
    double max_price = *std::max_element(path.begin(), path.end());
    return max_price - path.back();
}

double FixedLookbackCall::evaluate(const std::vector<double>& path) const {
    double max_price = *std::max_element(path.begin(), path.end());
    return std::max(max_price - K_, 0.0);
}

double FixedLookbackPut::evaluate(const std::vector<double>& path) const {
    double min_price = *std::min_element(path.begin(), path.end());
    return std::max(K_ - min_price, 0.0);
}
