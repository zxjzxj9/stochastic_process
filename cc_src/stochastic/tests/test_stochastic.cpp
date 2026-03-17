#include "test_runner.h"

#include "../rng.h"
#include "../process.h"
#include "../payoff.h"
#include "../mc.h"

#include <cmath>
#include <vector>
#include <numeric>

// ============================================================
// RNG tests
// ============================================================

TEST(rng_normal_mean_variance) {
    // N samples of N(0,1); check mean ~0, var ~1 to 2 decimal places
    RNG rng(123);
    const int N = 100000;
    double sum = 0, sum_sq = 0;
    for (int i = 0; i < N; ++i) {
        double x = rng.normal();
        sum += x;
        sum_sq += x * x;
    }
    double mean = sum / N;
    double var  = sum_sq / N - mean * mean;
    EXPECT_NEAR(mean, 0.0, 0.01);
    EXPECT_NEAR(var,  1.0, 0.02);
}

TEST(rng_normal_shifted) {
    RNG rng(42);
    const int N = 50000;
    double sum = 0;
    for (int i = 0; i < N; ++i)
        sum += rng.normal(5.0, 2.0);
    EXPECT_NEAR(sum / N, 5.0, 0.05);
}

TEST(rng_poisson_mean) {
    // Poisson(lam): sample mean should be ~lam
    RNG rng(7);
    const double lam = 3.0;
    const int N = 100000;
    double sum = 0;
    for (int i = 0; i < N; ++i)
        sum += rng.poisson(lam);
    EXPECT_NEAR(sum / N, lam, 0.05);
}

TEST(rng_seeded_reproducibility) {
    RNG a(999), b(999);
    for (int i = 0; i < 10; ++i)
        EXPECT_NEAR(a.normal(), b.normal(), 1e-15);
}

// ============================================================
// WienerProcess tests
// ============================================================

TEST(wiener_terminal_distribution) {
    // B(T) ~ N(0, T); check variance ~T using many samples
    WienerProcess W;
    RNG rng(1);
    const int N = 100000;
    const double T = 2.0;
    double sum_sq = 0;
    for (int i = 0; i < N; ++i) {
        double b = W.simulate_terminal(T, rng);
        sum_sq += b * b;
    }
    // E[B(T)^2] = T
    EXPECT_NEAR(sum_sq / N, T, 0.05);
}

TEST(wiener_path_starts_at_zero) {
    WienerProcess W;
    RNG rng(2);
    auto path = W.simulate_path(1.0, 100, rng);
    EXPECT_NEAR(path[0], 0.0, 1e-15);
    EXPECT_TRUE(path.size() == 101);
}

TEST(wiener_ito_isometry_chapter1) {
    // Verify Ito isometry: Var[int_0^1 s^2 dB_s] = int_0^1 s^4 ds = 0.2
    // Simulation: sum f(s_i) * dB_i  where f(s) = s^2
    RNG rng(42);
    const int ndiv = 1000;
    const double T = 1.0;
    const int n_trials = 10000;
    double dt = T / ndiv;
    double sqrt_dt = std::sqrt(dt);

    double sum_sq = 0;
    for (int trial = 0; trial < n_trials; ++trial) {
        double integral = 0;
        for (int i = 0; i < ndiv; ++i) {
            double s = i * dt;          // left endpoint
            double dB = sqrt_dt * rng.normal();
            integral += s * s * dB;
        }
        sum_sq += integral * integral;
    }
    double var = sum_sq / n_trials;
    // Theoretical variance = int_0^1 s^4 ds = 0.2
    EXPECT_NEAR(var, 0.2, 0.01);
}

// ============================================================
// GBMProcess tests
// ============================================================

TEST(gbm_terminal_lognormal) {
    // Under risk-neutral: log(S_T/S0) ~ N((r - sigma^2/2)*T, sigma^2*T)
    GBMProcess gbm(100.0, 0.05, 0.2);
    RNG rng(10);
    const int N = 100000;
    const double T = 1.0;

    double sum_log = 0, sum_log_sq = 0;
    for (int i = 0; i < N; ++i) {
        double log_return = std::log(gbm.simulate_terminal(T, rng) / 100.0);
        sum_log    += log_return;
        sum_log_sq += log_return * log_return;
    }
    double mean_log = sum_log / N;
    double var_log  = sum_log_sq / N - mean_log * mean_log;

    double expected_mean = (0.05 - 0.5 * 0.04) * T;   // (r - sigma^2/2)*T
    double expected_var  = 0.04 * T;                   // sigma^2 * T

    EXPECT_NEAR(mean_log, expected_mean, 0.01);
    EXPECT_NEAR(var_log,  expected_var,  0.01);
}

TEST(gbm_path_length) {
    GBMProcess gbm(100.0, 0.05, 0.2);
    RNG rng(11);
    auto path = gbm.simulate_path(1.0, 252, rng);
    EXPECT_TRUE(path.size() == 253);
    EXPECT_NEAR(path[0], 100.0, 1e-15);
}

TEST(gbm_paths_shape) {
    GBMProcess gbm(100.0, 0.05, 0.2);
    RNG rng(12);
    auto paths = gbm.simulate_paths(1.0, 50, 200, rng);
    EXPECT_TRUE(paths.size() == 200);
    for (const auto& p : paths) EXPECT_TRUE(p.size() == 51);
}

// ============================================================
// MertonProcess tests
// ============================================================

TEST(merton_compensator) {
    // kbar = exp(mu_J + 0.5*sigma_J^2) - 1
    MertonProcess m(100, 0.05, 0.15, 3.0, -0.05, 0.1);
    double expected = std::exp(-0.05 + 0.5 * 0.01) - 1.0;
    EXPECT_NEAR(m.compensator(), expected, 1e-12);
}

TEST(merton_terminal_positive) {
    MertonProcess m(100, 0.05, 0.15, 3.0, -0.05, 0.1);
    RNG rng(13);
    for (int i = 0; i < 1000; ++i)
        EXPECT_GT(m.simulate_terminal(1.0, rng), 0.0);
}

TEST(merton_paths_shape) {
    MertonProcess m(100, 0.05, 0.15, 3.0, -0.05, 0.1);
    RNG rng(14);
    auto paths = m.simulate_paths(1.0, 50, 100, rng);
    EXPECT_TRUE(paths.size() == 100);
    for (const auto& p : paths) EXPECT_TRUE(p.size() == 51);
}

// ============================================================
// Payoff tests
// ============================================================

TEST(european_call_payoff) {
    EuropeanCall call(100.0);
    EXPECT_NEAR(call.evaluate({90.0}),  0.0,  1e-12);
    EXPECT_NEAR(call.evaluate({110.0}), 10.0, 1e-12);
    EXPECT_NEAR(call.evaluate({100.0}), 0.0,  1e-12);
}

TEST(european_put_payoff) {
    EuropeanPut put(100.0);
    EXPECT_NEAR(put.evaluate({90.0}),  10.0, 1e-12);
    EXPECT_NEAR(put.evaluate({110.0}),  0.0, 1e-12);
}

TEST(digital_call_payoff) {
    DigitalCall dc(100.0);
    EXPECT_NEAR(dc.evaluate({99.0}),  0.0, 1e-12);
    EXPECT_NEAR(dc.evaluate({101.0}), 1.0, 1e-12);
}

TEST(asian_call_payoff) {
    // avg = (80 + 90 + 100 + 110 + 120) / 5 = 100; payoff = max(100-100,0)=0
    AsianCall ac(100.0);
    std::vector<double> path = {80, 90, 100, 110, 120};
    EXPECT_NEAR(ac.evaluate(path), 0.0, 1e-12);

    // avg = (90+110+130)/3 = 110; payoff = 10
    std::vector<double> path2 = {90, 110, 130};
    EXPECT_NEAR(ac.evaluate(path2), 10.0, 1e-12);
}

TEST(up_and_out_call) {
    UpAndOutCall uoc(100.0, 130.0);
    // Barrier NOT hit, S_T = 120: payoff = 20
    std::vector<double> path1 = {100, 110, 120};
    EXPECT_NEAR(uoc.evaluate(path1), 20.0, 1e-12);
    // Barrier hit (130): payoff = 0
    std::vector<double> path2 = {100, 130, 120};
    EXPECT_NEAR(uoc.evaluate(path2), 0.0, 1e-12);
}

TEST(down_and_out_put) {
    DownAndOutPut dop(100.0, 80.0);
    // Barrier NOT hit, S_T = 90: payoff = 10
    std::vector<double> path1 = {100, 95, 90};
    EXPECT_NEAR(dop.evaluate(path1), 10.0, 1e-12);
    // Barrier hit: payoff = 0
    std::vector<double> path2 = {100, 80, 90};
    EXPECT_NEAR(dop.evaluate(path2), 0.0, 1e-12);
}

TEST(floating_lookback_call) {
    FloatingLookbackCall flc;
    // S_T - min(S) = 120 - 80 = 40
    std::vector<double> path = {100, 80, 90, 120};
    EXPECT_NEAR(flc.evaluate(path), 40.0, 1e-12);
}

TEST(floating_lookback_put) {
    FloatingLookbackPut flp;
    // max(S) - S_T = 130 - 90 = 40
    std::vector<double> path = {100, 130, 110, 90};
    EXPECT_NEAR(flp.evaluate(path), 40.0, 1e-12);
}

// ============================================================
// Monte Carlo pricing tests
// ============================================================

TEST(mc_european_call_bs_benchmark) {
    // Black-Scholes call: S=K=100, r=0.05, sigma=0.2, T=1
    // BS price = 10.4506
    GBMProcess gbm(100.0, 0.05, 0.2);
    EuropeanCall call(100.0);
    auto result = mc_price(gbm, call, 1.0, 1, 500000, 42);

    EXPECT_NEAR(result.price, 10.4506, 0.15);   // within ~1.5% of BS
    EXPECT_GT(result.std_error, 0.0);
    EXPECT_TRUE(result.n_paths == 500000);
}

TEST(mc_put_call_parity) {
    // C - P = S*exp(0) - K*exp(-rT) (when S=K, r>0: C > P)
    GBMProcess gbm(100.0, 0.05, 0.2);
    EuropeanCall call(100.0);
    EuropeanPut  put(100.0);
    auto rc = mc_price(gbm, call, 1.0, 1, 500000, 42);
    auto rp = mc_price(gbm, put,  1.0, 1, 500000, 42);

    // Put-call parity: C - P = S0 - K*exp(-r*T) = 100 - 100*exp(-0.05) ~= 4.877
    double parity_theoretical = 100.0 - 100.0 * std::exp(-0.05);
    EXPECT_NEAR(rc.price - rp.price, parity_theoretical, 0.15);
}

TEST(mc_asian_call_cheaper_than_european) {
    // Asian options are always <= corresponding European option
    GBMProcess   gbm(100.0, 0.05, 0.2);
    EuropeanCall euro(100.0);
    AsianCall    asian(100.0);
    auto re = mc_price(gbm, euro,  1.0, 252, 200000, 42);
    auto ra = mc_price(gbm, asian, 1.0, 252, 200000, 42);
    EXPECT_LT(ra.price, re.price);
}

TEST(mc_barrier_call_cheaper_than_european) {
    GBMProcess  gbm(100.0, 0.05, 0.2);
    EuropeanCall euro(100.0);
    UpAndOutCall barrier(100.0, 130.0);
    auto re = mc_price(gbm, euro,    1.0, 252, 200000, 42);
    auto rb = mc_price(gbm, barrier, 1.0, 252, 200000, 42);
    EXPECT_LT(rb.price, re.price);
}

TEST(mc_lookback_call_pricier_than_european) {
    GBMProcess        gbm(100.0, 0.05, 0.2);
    EuropeanCall      euro(100.0);
    FloatingLookbackCall lookback;
    auto re = mc_price(gbm, euro,     1.0, 252, 200000, 42);
    auto rl = mc_price(gbm, lookback, 1.0, 252, 200000, 42);
    EXPECT_GT(rl.price, re.price);
}

TEST(mc_merton_call_pricier_than_gbm_call) {
    // Merton model with crash-like jumps: overall option prices differ
    // With lam=3, mu_J=-0.05: higher risk -> higher puts, modified calls
    GBMProcess    gbm(100.0, 0.05, 0.15);
    MertonProcess merton(100.0, 0.05, 0.15, 3.0, -0.05, 0.1);
    EuropeanCall  call(100.0);

    auto rg = mc_price(gbm,    call, 1.0, 1, 300000, 42);
    auto rm = mc_price(merton, call, 1.0, 1, 300000, 42);

    // Merton call price should differ from pure GBM (jumps add risk)
    // With negative mu_J the Merton call should be higher than lower-vol GBM
    EXPECT_GT(rm.price, rg.price);
}

TEST(mc_merton_terminal_vs_path) {
    // Terminal simulation (n_steps=1) and full path (n_steps=252) should be close
    MertonProcess merton(100.0, 0.05, 0.15, 3.0, -0.05, 0.1);
    EuropeanCall call(100.0);
    auto r1   = mc_price(merton, call, 1.0,   1, 300000, 42);
    auto r252 = mc_price(merton, call, 1.0, 252, 300000, 42);
    EXPECT_NEAR(r1.price, r252.price, 0.3);
}

// ============================================================
int main() {
    std::cout << "Running stochastic simulation library tests\n\n";
    return test_summary();
}
