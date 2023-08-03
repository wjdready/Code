
这次引入了 CortexM3, 配合 Cortex M3 权威指南看, 结合 ChatGPT 进行调试。

在这一版主要坑点是 VSCode 无法直接从入口 reset 打断点，不知为何。现在的解决办法是引入一个转跳指令，因为只有转跳后 VSCode 的断点才会识别。

因而程序入口变成:

```s
reset:
    b here
here:
    ldr sp, =_estack
```

这样 VSCode 只需设置断点 here 即可，适用于任何汇编位置。

CortexM3 使用的是 TI 的 lm3s6965evb 板子, 参考这个老哥的[博客记录](https://github.com/JiaminMa/write_rtos_in_3days.git) 以后就靠它了。

[FreeRTOS](https://www.freertos.org/cortex-m3-qemu-lm3S6965-demo.html#build_instructions_1)
