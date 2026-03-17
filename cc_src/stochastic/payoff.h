#pragma once

#include <vector>
#include <algorithm>
#include <numeric>

/**
 * Abstract base for path-dependent payoffs.
 * The path vector contains prices at each time step: [S(0), S(dt), ..., S(T)].
 */
struct Payoff {
    virtual double evaluate(const std::vector<double>& path) const = 0;
    virtual ~Payoff() = default;
};

// ============================================================
// European / terminal payoffs
// ============================================================

class EuropeanCall : public Payoff {
public:
    explicit EuropeanCall(double K) : K_(K) {}
    double evaluate(const std::vector<double>& path) const override;
private:
    double K_;
};

class EuropeanPut : public Payoff {
public:
    explicit EuropeanPut(double K) : K_(K) {}
    double evaluate(const std::vector<double>& path) const override;
private:
    double K_;
};

// Pays $1 if S(T) > K, else 0
class DigitalCall : public Payoff {
public:
    explicit DigitalCall(double K) : K_(K) {}
    double evaluate(const std::vector<double>& path) const override;
private:
    double K_;
};

// ============================================================
// Asian options (arithmetic average price)
// ============================================================

class AsianCall : public Payoff {
public:
    explicit AsianCall(double K) : K_(K) {}
    double evaluate(const std::vector<double>& path) const override;
private:
    double K_;
};

class AsianPut : public Payoff {
public:
    explicit AsianPut(double K) : K_(K) {}
    double evaluate(const std::vector<double>& path) const override;
private:
    double K_;
};

// ============================================================
// Barrier options (continuous monitoring)
// ============================================================

// Knocked out if max(S) >= barrier
class UpAndOutCall : public Payoff {
public:
    UpAndOutCall(double K, double barrier) : K_(K), barrier_(barrier) {}
    double evaluate(const std::vector<double>& path) const override;
private:
    double K_, barrier_;
};

// Knocked out if min(S) <= barrier
class DownAndOutPut : public Payoff {
public:
    DownAndOutPut(double K, double barrier) : K_(K), barrier_(barrier) {}
    double evaluate(const std::vector<double>& path) const override;
private:
    double K_, barrier_;
};

// ============================================================
// Lookback options
// ============================================================

// Floating lookback call: S(T) - min(S)
class FloatingLookbackCall : public Payoff {
public:
    double evaluate(const std::vector<double>& path) const override;
};

// Floating lookback put: max(S) - S(T)
class FloatingLookbackPut : public Payoff {
public:
    double evaluate(const std::vector<double>& path) const override;
};

// Fixed lookback call: max(max(S) - K, 0)
class FixedLookbackCall : public Payoff {
public:
    explicit FixedLookbackCall(double K) : K_(K) {}
    double evaluate(const std::vector<double>& path) const override;
private:
    double K_;
};

// Fixed lookback put: max(K - min(S), 0)
class FixedLookbackPut : public Payoff {
public:
    explicit FixedLookbackPut(double K) : K_(K) {}
    double evaluate(const std::vector<double>& path) const override;
private:
    double K_;
};
