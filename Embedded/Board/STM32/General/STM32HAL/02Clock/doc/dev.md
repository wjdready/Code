

# [2023-03-16 12:51:00]

每次想手搓裸机代码时，时钟总是一个越不过去的坎，时钟配置之所以搞得很复杂，原因是外设太丰富了，以前 8086 只要给 CPU 晶振，外设都是自己自定义设计的。MCU 外设丰富，每个外设都需要时钟管理，时钟配置本身也是一个外设或说寄存器，比如 STM32 的 RCC。

上电一瞬间，时钟肯定没有配置，这时也能运行程序，这是默认时钟，仅初始化了最基本的外设，和 CPU 时钟。而且还是低速模式。
