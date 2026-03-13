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

### 1. Heat Equation Simulation (`cc_src/`)

Numerical simulation of the heat equation on a 2D circular domain using finite differences (explicit Euler method). The boundary is a unit circle with fixed temperature, and the interior evolves according to:

$$\frac{\partial T}{\partial t} = -\frac{1}{2} \nabla^2 T$$

The C++ solver is exposed to Python via **pybind11** for visualization with matplotlib.

### 2. Quantitative Finance (`quant/`)

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
