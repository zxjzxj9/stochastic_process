# Quant Source

This folder contains quantitative finance implementations built from first principles.

---

## 1. Vanilla Option Pricing (Black-Scholes)

We derive the Black-Scholes formula for pricing European vanilla options.

### 1.1 Geometric Brownian Motion

Suppose the underlying asset price $S$ follows a geometric Brownian motion, where $W$ is a standard Wiener process:

$$\frac{dS}{S} = \mu \, dt + \sigma \, dW$$

### 1.2 Ito's Lemma

To find the dynamics of a function of $S$, we use **Ito's Lemma**. Given a Brownian motion:

$$dX_t = \mu_t \, dt + \sigma_t \, dW_t$$

For any twice-differentiable function $f(X_t, t)$, Ito's Lemma gives:

$$df = \frac{\partial f}{\partial t} \, dt + \mu_t \frac{\partial f}{\partial X_t} \, dt + \frac{\sigma_t^2}{2} \frac{\partial^2 f}{\partial X_t^2} \, dt + \sigma_t \frac{\partial f}{\partial X_t} \, dW_t$$

### 1.3 Deriving the Black-Scholes PDE

Let $V(S, t)$ be the option price as a function of $S$ and $t$. Applying Ito's Lemma to $V$:

$$dV = \frac{\partial V}{\partial t} \, dt + \mu S \frac{\partial V}{\partial S} \, dt + \frac{1}{2} \sigma^2 S^2 \frac{\partial^2 V}{\partial S^2} \, dt + \sigma S \frac{\partial V}{\partial S} \, dW_t$$

Construct a **delta-hedging portfolio** $P$ to eliminate the stochastic term:

$$P = -V + \frac{\partial V}{\partial S} \cdot S$$

Its differential is:

$$dP = -dV + \frac{\partial V}{\partial S} \, dS = -\frac{\partial V}{\partial t} \, dt - \frac{1}{2} \sigma^2 S^2 \frac{\partial^2 V}{\partial S^2} \, dt$$

Note that $dP$ has no $dW_t$ term — the portfolio is **risk-free**. By the no-arbitrage principle:

$$rP \, dt = dP$$

Substituting and rearranging, we obtain the **Black-Scholes PDE**:

$$\frac{\partial V}{\partial t} + rS \frac{\partial V}{\partial S} + \frac{1}{2} \sigma^2 S^2 \frac{\partial^2 V}{\partial S^2} - rV = 0$$

### 1.4 Black-Scholes Formula

Solving the PDE with appropriate boundary conditions yields the closed-form pricing formula.

**Call price:**

$$C(S, t) = S \cdot \Phi(d_1) - K e^{-rT} \cdot \Phi(d_2)$$

**Put price** (via put-call parity):

$$P(S, t) = K e^{-rT} \cdot \Phi(-d_2) - S \cdot \Phi(-d_1)$$

where $\Phi(\cdot)$ is the standard normal CDF:

$$\Phi(x) = \frac{1}{2}\left(1 + \mathrm{erf}\!\left(\frac{x}{\sqrt{2}}\right)\right)$$

and

$$d_1 = \frac{\ln(S/K) + (r + \sigma^2/2) \, T}{\sigma \sqrt{T}}, \qquad d_2 = d_1 - \sigma\sqrt{T}$$

The implementation is in `vanilla_option.cpp`.

---

## 2. Monte Carlo Simulation

Starting from the risk-neutral dynamics:

$$dS = rS \, dt + \sigma S \, dW$$

Applying Ito's Lemma to $\ln S$:

$$d\ln S = \frac{1}{S} \, dS - \frac{1}{2}\sigma^2 \, dt = \left(r - \frac{1}{2}\sigma^2\right) dt + \sigma \, dW$$

Integrating over $[0, T]$ gives the exact solution:

$$S_T = S_0 \exp\!\left[\left(r - \frac{1}{2}\sigma^2\right)T + \sigma\sqrt{T} \, Z\right], \quad Z \sim N(0, 1)$$

The option price under the risk-neutral measure is:

$$V = e^{-rT} \, \mathbb{E}\!\left[f(S_T)\right]$$

where $f$ is the payoff function (e.g., $\max(S_T - K, 0)$ for a call). We estimate this expectation by averaging over many simulated paths.

---

## 3. Option Greeks

The Greeks measure the sensitivity of the option price to various parameters. Below, $\phi(x)$ denotes the standard normal PDF and $\Phi(x)$ the standard normal CDF.

| Greek | Definition | Call | Put |
|:------|:----------:|:----:|:---:|
| **Delta** | $\dfrac{\partial V}{\partial S}$ | $\Phi(d_1)$ | $\Phi(d_1) - 1$ |
| **Gamma** | $\dfrac{\partial^2 V}{\partial S^2}$ | $\dfrac{\phi(d_1)}{S \sigma \sqrt{T}}$ | $\dfrac{\phi(d_1)}{S \sigma \sqrt{T}}$ |
| **Vega** | $\dfrac{\partial V}{\partial \sigma}$ | $S \, \phi(d_1) \sqrt{T}$ | $S \, \phi(d_1) \sqrt{T}$ |
| **Theta** | $\dfrac{\partial V}{\partial t}$ | $-\dfrac{S \sigma \, \phi(d_1)}{2\sqrt{T}} - rKe^{-rT}\Phi(d_2)$ | $-\dfrac{S \sigma \, \phi(d_1)}{2\sqrt{T}} + rKe^{-rT}\Phi(-d_2)$ |
| **Rho** | $\dfrac{\partial V}{\partial r}$ | $KT \, e^{-rT} \Phi(d_2)$ | $-KT \, e^{-rT} \Phi(-d_2)$ |

Note:
- **Gamma** and **Vega** are the same for calls and puts.
- $\phi(x) = \frac{1}{\sqrt{2\pi}} e^{-x^2/2}$ is the standard normal PDF.
- $\Phi(x) = \frac{1}{2}\left(1 + \mathrm{erf}\!\left(\frac{x}{\sqrt{2}}\right)\right)$ is the standard normal CDF.

The implementation is in `greeks.cpp`. Delta, Gamma, Vega, and Theta are currently implemented with analytical formulas. Rho and Monte Carlo-based Greeks are planned for future work.

---

## 4. Implied Volatility

Given an observed market option price $C_{\text{market}}$, the **implied volatility** (IV) is the value of $\sigma$ that satisfies:

$$C_{\text{BS}}(S, K, r, T, \sigma) = C_{\text{market}}$$

We solve this using the **Newton-Raphson** method. Since we already have the Vega ($\partial C / \partial \sigma$), the iteration is:

$$\sigma_{n+1} = \sigma_n - \frac{C_{\text{BS}}(\sigma_n) - C_{\text{market}}}{\text{Vega}(\sigma_n)}$$

This converges quadratically. The same method works for both calls and puts — only the pricing function changes, while Vega is identical for both.

The implementation is in `implied_vol.cpp`.
