


[ARM官网](https://developer.arm.com/documentation/dui0472/k/Compiler-specific-Features/Predefined-macros) 提供了很多 ARMCC 的内置宏

[Cubemx官方使用手册](https://www.st.com/resource/en/user_manual/um1718-stm32cubemx-for-stm32-configuration-and-initialization-c-code-generation-stmicroelectronics.pdf)


对于一些编译器的扩展宏:
```
__no_init 意味着编译器不应该初始化变量，并且当变量被放置在硬件寄存器位置并且您不希望程序启动时将寄存器设置为 0 时使用。
__root    用于告诉编译器/链接器变量或函数应该保存在二进制映像中，即使它实际上没有在程序中使用。这对于调试目的和在开发过程中“调整”程序可能很有用。
```


[嵌入式高手jaycarlson的博客](https://jaycarlson.net/embedded-linux/)

