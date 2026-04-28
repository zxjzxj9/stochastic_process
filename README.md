# Stochastic Process — Numerical Experiments

A collection of numerical experiments and implementations related to stochastic processes and quantitative finance.

## Option Pricing Primer

A condensed reference for the concepts the notebooks experiment with.

### What is an option?

A **call** gives the holder the right (not obligation) to buy an asset at strike $K$ on or before expiry $T$; a **put** gives the right to sell. **European** options can be exercised only at $T$; **American** options at any time $\tau \le T$. Payoffs at expiry:

$$
\text{Call: } (S_T - K)^+ \qquad \text{Put: } (K - S_T)^+
$$

### Risk-neutral pricing

Under the risk-neutral measure $\mathbb{Q}$, the discounted price is a martingale, so

$$
V_0 = e^{-rT}\, \mathbb{E}^{\mathbb{Q}}\!\left[\, \text{payoff}(S_T) \,\right].
$$

The underlying follows a geometric Brownian motion (GBM) with drift $r$ replacing the real-world drift:

$$
dS_t = r\, S_t\, dt + \sigma\, S_t\, dW_t.
$$

### Black-Scholes formula

For a European call with constant $r, \sigma$:

$$
C = S_0\, \Phi(d_1) - K e^{-rT}\, \Phi(d_2), \qquad d_{1,2} = \frac{\ln(S_0/K) + (r \pm \tfrac{1}{2}\sigma^2)T}{\sigma\sqrt{T}}.
$$

**Put-call parity:** $\;C - P = S_0 - K e^{-rT}$.

### The Greeks

Sensitivities of the option price to its inputs:

| Greek | Definition | Meaning |
|-------|------------|---------|
| Delta $\Delta$ | $\partial V / \partial S$ | Sensitivity to spot |
| Gamma $\Gamma$ | $\partial^2 V / \partial S^2$ | Curvature / Delta sensitivity |
| Vega $\nu$ | $\partial V / \partial \sigma$ | Sensitivity to volatility |
| Theta $\Theta$ | $\partial V / \partial t$ | Time decay |
| Rho $\rho$ | $\partial V / \partial r$ | Sensitivity to rates |

### Numerical methods used in this repo

- **Feynman-Kac / PDE** — the price satisfies the Black-Scholes PDE; solving it backward from the payoff terminal condition is equivalent to taking a risk-neutral expectation (chapters 3-4).
- **Monte Carlo** — simulate many GBM paths, average the discounted payoff. Natural for path-dependent options (Asian, barrier, lookback in chapter 6) and jumps (Merton in chapter 7).
- **Binomial tree (CRR)** — discretize $S$ on an up/down lattice; back-induct the payoff. Handles American early exercise cleanly (chapter 8).
- **Longstaff-Schwartz** — Monte Carlo for American options: regress continuation value against basis functions to decide exercise (chapter 5).

### Implied volatility & beyond Black-Scholes

Black-Scholes assumes constant $\sigma$, but market option prices implied a **volatility smile/skew** across strikes. Chapters 7-12 explore how to model and fit this:

- **Merton jump-diffusion** (ch. 7) — adds Poisson jumps; produces a smile.
- **Heston** (ch. 9) — stochastic volatility $dv_t = \kappa(\theta - v_t)\,dt + \xi\sqrt{v_t}\,dW_t^v$ with $\mathrm{corr}(dW^S, dW^v) = \rho$.
- **SVI** (ch. 10) — Gatheral's parameterization of total implied variance $w(k) = a + b[\rho(k-m) + \sqrt{(k-m)^2 + \sigma^2}]$, used to interpolate the surface.
- **GARCH** (ch. 11) — time-series model of conditional variance for forecasting realized volatility.
- **Surface in practice** (ch. 12) — fitting and arbitrage-checking the full $(K, T)$ volatility surface.

## Contents

### Jupyter Notebooks

| Notebook | Topic |
|----------|-------|
| `chapter1.ipynb` | Stochastic integral of deterministic functions w.r.t. Wiener process |
| `chapter2.ipynb` | Stochastic integral of random-variable functions (Ito calculus) |
| `chapter3.ipynb` | Connection between stochastic processes and PDEs (Feynman-Kac) |
| `chapter4.ipynb` | European option pricing via Feynman-Kac Monte Carlo |
| `chapter5.ipynb` | American option pricing via Longstaff-Schwartz |
| `chapter6.ipynb` | Path-dependent options (Asian, barrier, lookback) |
| `chapter7.ipynb` | Jump-diffusion model (Merton) and volatility smile |
| `chapter8.ipynb` | Binomial tree option pricing (Cox-Ross-Rubinstein) |
| `chapter9.ipynb` | Heston stochastic volatility model |
| `chapter10.ipynb` | SVI parameterization of the implied volatility surface |
| `chapter11.ipynb` | GARCH model and volatility forecasting |
| `chapter12.ipynb` | Volatility surface and curve in practice |

### 1. Heat Equation Simulation (`cc_src/`)

Numerical simulation of the heat equation on a 2D circular domain using finite differences (explicit Euler method). The boundary is a unit circle with fixed temperature, and the interior evolves according to:

$$\frac{\partial T}{\partial t} = -\frac{1}{2} \nabla^2 T$$

The C++ solver is exposed to Python via **pybind11** for visualization with matplotlib.

### 2. Stochastic Simulation Library (`cc_src/stochastic/`)

C++ library for Monte Carlo simulation of stochastic processes, exposed to Python via **pybind11** as a `stochastic` module:

- Random number generation (`rng.h`)
- Process simulators (`process.h/cpp`) — GBM and related SDE paths
- Payoff functors (`payoff.h/cpp`)
- Monte Carlo pricing engine (`mc.h`) returning price, standard error, and path count
- Unit tests under `cc_src/stochastic/tests/`

### 3. Quantitative Finance (`quant/`)

Implementation of the **Black-Scholes** option pricing model, including:

- Analytical call/put pricing via the Black-Scholes formula
- Monte Carlo simulation of vanilla options
- Option Greeks: Delta, Gamma, Vega, Theta, Rho

See [`quant/README.md`](quant/README.md) for the full derivation and formulas.

## Building

### Heat Equation (cc_src)

```bash
cd cc_src
mkdir build && cd build
cmake ..
make
```

### Quant Library (quant)

```bash
cd quant
mkdir build && cd build
cmake ..
make
```
