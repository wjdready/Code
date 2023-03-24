# ARM GNU 编译与链接02: GNU 汇编基础

[RedHat上的文档](http://web.mit.edu/rhel-doc/3/rhel-as-en-3/index.html)

在 02UseLibc 的基础稍微修改即可在 QEMU 上跑, 这里使用的 QEMU 板子是 `vexpress-a9`，其初始内存地址是 

QEMU 加载 elf 文件会直接跳到 ld 中 ENTRY 指定 标签所在地址，因此可以忽略向量表的地址转跳也可以运行

QEMU 的 `vexpress-a9` 板子默认会将程序加载到 0x60010000 内存地址运行，这里将以单片机类似的写法，将前 128KB 设为只读代码区，后面 128KB 设为 RAM 区:

```
ROM (rx)            : ORIGIN = 0x60010000, LENGTH = 128K
RAM (xrw)           : ORIGIN = 0x60020000, LENGTH = 128K
```

Makefile 中更改 CPU 类型为 armv7-a，同时 QEMU 启动时不要忘记使用 半主机模式:

```
qemu-system-arm -M vexpress-a9 -kernel build/demo.elf -semihosting -S -s
```

最后 GDB 中要设置服务器地址端口为 3333, 并通知 GDB 使能半主机模式:

```
{"text": "target extended-remote localhost:3333"},
{"text": "monitor arm semihosting enable"},
```

其他的基本和 `02UseLibc` 的一样，这样一来就可以抛开板子直接研究 ARM 汇编和 Link Scripts 了。

还有更多扩展思维的空间，比如 

* QEMU 中仿真 Cortex-M, 比如 STM32 的板子;
* 使用 bin 格式文件加载;
* 链接脚本如何使用动态的方式；
* 通过 u-boot 加载；
* QEMU 添加更多外设；
* QEMU 中添加自定义外设；
* 驱动 vexpress-a9 的 UART 或者 GUI；
* 通用 ARM 的仿真 virtix ARM；
