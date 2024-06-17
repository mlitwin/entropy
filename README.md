# Entropy

## Circle Problem

```
        3  4
     2        5
    1          6
    0          7
     N-1      8
        .. 9
```

Analyze the behavior of particles moving in a fixed circle. Take positions, velocities, and time to be discrete, in $\mathbb{Z}$.

### Definitions

$$
\newcommand{\idiv}{\operatorname{div}}
\newcommand{\sgn}{\operatorname{sgn}}
$$

- $0 \ldots  {N-1}$ sensor positions
- Velocities in the range -V to V inclusive
- Throughout take $t, k, v \in \mathbb{Z}$
- Write $a \idiv b$ for integer division $\lfloor \frac{a}{b} \rfloor$
- $a \bmod N$ is positive

Let $d_t(k;v) =$ Number of particles at time $t$ with position $k$ and velocity $v$.

Let $\rho_t(k) = \sum_v d_t(k;v)$ be the observed density at at time $t$ at position $k$.

### Basics

$d_t(k,v) = d_0(({k+v t})\bmod N;v)$, and thus $\rho_t$ has period N: $\rho_t(k) = \rho_{t+a N}(k)$

### States

Let $D_t(k;s) = \rho_t(k) \idiv s$ be the density at time $t$ position $k$ with sensitivity $s$

Fix $s$

Since $\rho_t$ is periodic with period N, then $S_t= \{D(k)\}_t$ are the N (possibly repeated) available states.

Let $J_t = $ the number of times state $S_t$ occurs.

### Arrow of time

#### Jitter

Motivation: if you see a transition from a rare state, to a less rare state, to a common state, maybe that means time is moving forward.

Let
$$J_s = \sum_k{\sgn(J_{k+1}-J_k)}$$

# Simulation

./circle -n 2000 -v 30 -p 1

## Visualization

https://mlitwin.github.io/entropy/
