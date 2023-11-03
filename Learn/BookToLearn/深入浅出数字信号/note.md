
典型信号

1. 单位冲激信号

$$ 
\delta (n) = 
\left\{
\begin{array}{l}
    1, n = 0 \\
    0, n \neq 0
\end{array}
\right.
$$

2. 单位阶跃信号

$$ 
u(n) = 
\left\{
\begin{array}{l}
    1, n \geqslant 0 \\
    0, n < 0
\end{array}
\right.
$$

可以看出单位冲击是单位阶跃的一次差分

$$ \delta (n) = u(n) - u(n - 1) $$

单位阶跃是单位脉冲的求和

$$ u(n) = \sum_{m = -\infty}^{n} \delta(m) $$

