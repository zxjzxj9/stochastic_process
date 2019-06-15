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

$$ dV = \frac{\partial V}{\partial t} dt + \mu S \frac{\partial V}{\partial S} dt + 
\frac{1}{2} \sigma^2 S^2 \frac{\partial^2 V}{\partial S^2}dt
+\sigma S\frac{\partial V}{\partial S}  dW_t
$$

Then we use delta-hedge portfolio, which can be stated as follows,
$$
P = -V + S\frac{\partial V}{\partial S}
$$

$$
dP = -dV + \frac{\partial V}{\partial S} dS=-\frac{\partial V}{\partial t} dt -\frac{1}{2} \sigma^2 S^2 \frac{\partial^2 V}{\partial S^2}dt
$$

Then we suppose the portfolio is risk free, which indicates,
$$
rPdt = dP
$$
So in concusion, we have the following equation
$$
r(-V + S\frac{\partial V}{\partial S}) = -\frac{\partial V}{\partial t}  -\frac{1}{2} \sigma^2 S^2 \frac{\partial^2 V}{\partial S^2}
$$
By rearranging the equation, we obtain the following parabolic PDE,
$$
\frac{\partial V}{\partial t}+rS\frac{\partial V}{\partial S}+\frac{1}{2} \sigma^2 S^2 \frac{\partial^2 V}{\partial S^2} -rV = 0
$$
So we can solve the PDE, and finally obtain,
$$
V(S,t) = S \cdot erf(d_1) - Ke^{-rT} \cdot erf(d_2) 
$$
where 
$$
d_1 = \frac{ln(S/K)+(r+\sigma^2/2)T}{\sigma\sqrt{T}}
$$
$$
d_2 = \frac{ln(S/K)+(r-\sigma^2/2)T}{\sigma\sqrt{T}}
$$
We have the code in the class vanilla_option.

### **2. Monte Carlo Simulation of Vanilla option**

Starting with the above formula, we will start the deriation of Monte Carlo simulation of vanilla option. With the following stochastic process, 

$$\frac{dS}{S} = \mu dt + \sigma dW$$

According to Ito's formula, we have the following SDE,
$$ d \ln S = \left(\mu - \frac{1}{2}\sigma^2\right)S + \sigma dW_t$$

Therefore we can simulate the trajectory of S w.r.t the change of of the time.
