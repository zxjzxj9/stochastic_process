# Quant Source

### This folder contains a series of quant code

### **1. Vanilla option**

This folder includes the vanilla option price calculated by Black-Scholes model.
Here we will derive the model as follows.
Suppose we have a Browian motion with the follow form, where S is the underlying asset value, and W is the Wiener process. 

$$\frac{dS}{S} = \mu dt + \sigma dW$$

According to Ito's lemma, we can calculate the analytic form of stochastic process S. Let me rephrase Ito's lemma as follows.

Give a brownian motion, where $W_t$ is a Wiener process,
$$ dX_t = \mu_t dt + \sigma_t dW_t$$
Let $f(x, t)$ be a function of random variable $X_t$ and t, then we have
$$ df(X_t, t) = \frac{\partial f}{\partial t}dt + 
	\mu_t \frac{\partial f}{\partial X_t} dt+ 
	\frac{\sigma_t^2}{2} \frac{\partial^2 f}{\partial X_t^2}dt +   
	\sigma_t \frac{\partial f}{\partial x} dW_t $$
What we want to know the the evolution of target price K as function of S and t, so we have the final formula like follows.

$$ dK = \frac{\partial K}{\partial t} dt + \mu S \frac{\partial K}{\partial S} dt + 
\frac{1}{2} \sigma^2 S^2 \frac{\partial^2 K}{\partial S^2}dt
+\sigma S\frac{\partial K}{\partial S}  dW_t
$$

Then we use delta-hedge portfolio, which can be stated as follows,
$$
P = -K + S\frac{\partial K}{\partial S}
$$

$$
dP = -dK + \frac{\partial K}{\partial S} dS=-\frac{\partial K}{\partial t} dt -\frac{1}{2} \sigma^2 S^2 \frac{\partial^2 K}{\partial S^2}dt
$$

Then we suppose the portfolio is risk free, which indicates,
$$
rPdt = dP
$$
So in concusion, we have the following equation
$$
r(-K + S\frac{\partial K}{\partial S}) = -\frac{\partial K}{\partial t}  -\frac{1}{2} \sigma^2 S^2 \frac{\partial^2 K}{\partial S^2}
$$
By rearranging the equation, we obtain the following parabolic PDE,
$$
\frac{\partial K}{\partial t}+rS\frac{\partial K}{\partial S}+\frac{1}{2} \sigma^2 S^2 \frac{\partial^2 K}{\partial S^2} -rK = 0
$$
So we can solve the PDE, and finally obtain,
