
关于 ARM 汇编快速入门，请 [参考](https://www.ic.unicamp.br/~celio/mc404-2014/docs/gnu-arm-directives.pdf)
# 使用 C

## 初始化栈

进入 C 的世界栈是必须的，在定义中断向量表时设置了第一个 word 的值就是栈的初始地址 _estack，

```s
.section  .isr_vector
    .word _estack
    .word reset
```

_estack 在 ld 文件中被指向为 DTCMRAM 的末尾，因为 ARM 体系中，栈是向低地址伸长，因此需要放在内存末尾处：
```
_estack = ORIGIN(DTCMRAM) + LENGTH(DTCMRAM);
```

中断向量表会被连接到 FLASH 的最开始地址：

```ld
/* 段组织 */
SECTIONS
{
    .my_vertor :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } >FLASH

    ...
}
```

STM32 复位后，BOOT=0 时，CPU 会从 FLASH 地址的第一个 word 处加载 sp 指针，从第二个 word 处加载 pc 指针，从而转跳到用户代码，因此这里只需要在中断向量表的第一个 word 填入栈地址，C 的环境便建立好了。

但是一般来说，为了保险，在进入用户代码的第一时间，我们还是需要主动设置一下 sp 地址，因为有可能 reset 函数不是通过硬件复位进去的，这时 sp 可能已经更改，而且由于是软件调用复位， CPU 不会自动加载向量表中的 sp 值，因此需要第一时间重写设置 sp 寄存器：
```as
/* 代码段 */
.section .text
.type reset, %function
.globl  reset

/* 程序入口 */
reset:
    ldr sp, =_estack
```

> ldr 属于 THUMB 指令格式，需要在顶部指示 .syntax unified，说明下面的指令是 ARM 和 THUMB 通用格式的

## 初始化内存变量

虽然这时候可以进入 C 的环境了，但是 C 中用到的内存和变量都还是随机，并没有进行初始化和赋值。这样一来，C 中定义的一些全局变量可能无法正常使用，因此这一步需要在汇编中将变量进行初始化和赋值。

一般来说，为了方便一次性对各个 C 文件中的静态或全局变量进行初始化，在链接器中需要对它们进行合理的安排：

* 首先是只读数据，只读数据在编译阶段就已经确定其值且不会再更改，因此我们可以把只读数据放到 FLASH 中，这样就不需要占用内存了, 同时在启动代码中也不需要对其进行赋值和初始化：
```ld
/* 将声明为 const 的数据放到 FLASH */
.rodata :
{
    . = ALIGN(4);
    *(.rodata)
    *(.rodata*)
    . = ALIGN(4);
} >FLASH
```

* 对于有设置了非0初值的可读写数据需要同时放到内存以及 FLASH, 需要同时放到 FLASH 和内存，然后在启动代码中，将 FLASH 中的初值赋值到内存，从而完成变量的初始化。

```ld
/* 将读写数据段放到 RAM */
.data : 
{
    . = ALIGN(4);
    _sdata = .;        /* 在 data 开头定义一个全局标签 _sdata 以便在汇编中访问 */

    *(.data)
    *(.data*)

    . = ALIGN(4);
    _edata = .;        /* 在 data 结尾定义一个全局标签 _edata 以便在汇编中访问 */
} >DTCMRAM AT> FLASH
```



# 02 使用 newlib 堆

[参考](https://llvm-gcc-renesas.com/wiki/index.php?title=How_does_Newlib_initialize_the_heap%3F)

# 01 Map 文件

[参考](https://interrupt.memfault.com/blog/get-the-most-out-of-the-linker-map-file)

## Glibc 中的特殊的段

| 段名  | 备注                                                            |
| ----- | --------------------------------------------------------------- |
| .init | 一个函数放到。init 段，在 main 函数执行前系统就会执行它             |
| .fini | 假如一个函数放到。fini 段，在 main 函数返回后该函数就会被执行       |

## __libc_init_array 这个函数中执行的关键过程如下：

调用 .preinit_array 段中的预初始化函数
调用 .init 段中的 _init 函数
调用 .init_array 中的所有函数

这里我需要解释下，在一个段中可能会存在多个函数，根据链接脚本的写法，链接器会在链接时将段名相同的函数指针放到同一个段中，然后通过在段的前后设定锚点来依次执行相应的函数，这样的方式在一些系统的驱动初始化中也能见到！

与 __libc_init_array 对应的函数是 __libc_fini_array，这个函数是在 main 函数执行完成后执行的。它首先会调用 .fini_array 中的所有函数，然后调用 _fini 函数，在嵌入式中一般不会执行到 __libc_fini_array 函数。

在 pc 端也有类似的过程，在我的 debian 10 中 _init 与 _fini 函数在 crti.o 中被定义，详细的信息如下
