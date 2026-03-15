# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Educational repository on stochastic processes and quantitative finance. Contains Jupyter notebooks (chapters 1-7) progressing from stochastic integrals through exotic option pricing, plus two C++ libraries exposed to Python.

## Build Commands

### Heat equation solver (cc_src/)
```bash
cd cc_src && mkdir -p build && cd build && cmake .. && make
```
Requires pybind11 as a git submodule in `cc_src/pybind11/`. Produces a Python module `heat` for use in chapter3.

### Quant library (quant/)
```bash
cd quant && mkdir -p build && cd build && cmake .. && make
```
Produces `main.x` executable. C++11, no external dependencies beyond standard math.

### Running notebooks
```bash
jupyter nbconvert --to notebook --execute chapterN.ipynb
```
chapter3 depends on the `heat` pybind11 module being built first.

## Architecture

### Notebooks (chapter1-7)
Sequential progression — each chapter builds on prior concepts:
1. Stochastic integrals of deterministic functions (Wiener process)
2. Ito calculus (stochastic integrals of random functions)
3. Feynman-Kac formula — PDE/stochastic process connection; uses C++ `heat` module
4. European option pricing via Feynman-Kac Monte Carlo
5. American option pricing via Longstaff-Schwartz
6. Path-dependent options (Asian, barrier, lookback)
7. Merton jump-diffusion model and volatility smile

### C++ code
- **cc_src/**: Heat equation PDE solver on 2D circular domain (explicit Euler finite differences), exposed to Python via pybind11
- **quant/**: Black-Scholes pricing library
  - `vanilla_option.h/cpp` — analytical call/put pricing
  - `greeks.h/cpp` — Delta, Gamma, Vega, Theta, Rho (classes inheriting option type)
  - `implied_vol.h/cpp` — Newton-Raphson implied volatility solver
  - `utils.h` — normal PDF/CDF helpers
  - `payoff.h/cpp` — call/put payoff functors

## Key Patterns

- Notebooks use `%matplotlib inline` and are self-contained (define all functions inline)
- MC pricing pattern: simulate terminal/path prices under risk-neutral measure, evaluate payoff, discount and average
- C++ quant code uses class hierarchy: `call`/`put` inherit from base option types in greeks
- Use raw strings for matplotlib LaTeX labels (e.g., `r'$\sigma$'`) to avoid SyntaxWarnings

## Git Remote

Uses SSH: `git@github.com:zxjzxj9/stochastic_process.git`
