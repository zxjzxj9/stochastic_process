# Stochastic Process — Numerical Experiments

A collection of numerical experiments and implementations related to stochastic processes and quantitative finance.

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
