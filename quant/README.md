# Quant Source

### This folder contains a series of quant code

### **1. Vanilla option**

This folder includes the vanilla option price calculated by Black-Scholes model.
Here we will derive the model as follows.
Suppose we have a Browian motion with the follow form, where S is the target price, and W is the Wiener process. 

$$\frac{dS}{S} = \mu dt + \sigma dW$$

According to Ito's lemma, we can calculate the analytic form of stochastic process S. Let me rephrase Ito's lemma as follows.gi