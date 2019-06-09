# Quant Source

### This folder contains a series of quant code

### **1. Vanilla option**

This folder includes the vanilla option price calculated by Black-Scholes model.
Here we will derive the model as follows.
Suppose we have a Browian motion with the follow form, where S is the target price, and W is the Wiener process. 

$$\frac{dS}{S} = \mu dt + \sigma dW$$

According to Ito's lemma, we can calculate the analytic form of stochastic process S. Let me rephrase Ito's lemma as follows.

Give a brownian motion, where $W_t$ is a Wiener process,
$$ dX_t = \mu_t dt + \sigma_t dW_t$$
Let $f(x, t)$ be a function of random variable $X_t$ and t, then we have
$$ df(X_t, t) = \frac{\partial f}{\partial t}dt + 
	\mu_t \frac{\partial f}{\partial X_t} dt+ 
	\frac{\sigma_t^2}{2} \frac{\partial^2 f}{\partial X_t^2}dt +   
	\sigma_t \frac{\partial f}{\partial x} dB_t $$
What we want to know the the evolution of 