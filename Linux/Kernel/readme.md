

Linux 内核官网 [kernel.org](https://kernel.org/)

首先应当先看[官方文档](kernel.org/doc), 很多问题都可以在官方文档中得到解决.


内核调试技巧:

1. 在不同厂家编译框架中，暂时理不清但是有需要单独编译内核，可在 Makefile 中添加 kernel 目标，然后执行 make -C kernel_dir, 这样由于是在 Makefile 中环境是已经配好了
2. 追踪函数调用可以添加 dump_stack(), 从而了解调用过程，然后根据给的地址，判断调用的函数指针所对应的位置.
3. 内核配置时, 先根据 `make your_defconfig` 覆盖得到 .config, 然后再在 .config 的基础上修改，修改完成后再使用 `make savedefconfig` , 就会在 kernel 文件夹中生成更改后的最小化 defconfig, 再拿去覆盖老的 your_defconfig
4. 对于 buildroot 则会直接覆盖原始文件 configs/your_defconfig
5. 音频驱动可以先生成一个简单的虚拟声卡比如 `mysimplecard`，然后使用 i2cset 的方式配置 adc， 并测试硬件, 当摸清楚 adc 配置后再写成驱动
