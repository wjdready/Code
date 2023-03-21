
---
mytags: myblog
title: 02UseLibc
date: 2023-03-21 09:46:06
categories: [ARMGNU]
tags: [libc, 半主机]
---

本文主要介绍在ARM汇编中使用 Libc 和半主机调试
<!-- more -->

关于 ARM 汇编快速入门，请 [参考](https://www.ic.unicamp.br/~celio/mc404-2014/docs/gnu-arm-directives.pdf)

start.s

```asm

.syntax unified

/* 代码段 */
.section .text.reset
.type reset, %function
.globl  reset

/* 程序入口 */
reset:

    ldr sp, =_estack

    /* 将 data 段从 flash 拷贝到 SRAM */
    ldr r0, =_data_start
    ldr r1, =_data_end
    ldr r2, =_init_data_start
    movs r3, #0
    b copy_data_loop

copy_data:
    ldr r4, [r2, r3]    // r4 = *(_init_data_start + r3)
    str r4, [r0, r3]    // *(_data_start + r3) = r4
    adds r3, r3, #4     // r3 += 4

copy_data_loop:
    adds r4, r0, r3     // r4 = _data_start + r3
    cmp r4, r1         
    bcc copy_data       // if(r4 != r1) copy_data

    /* 将 0 填充到 .bss 段中 */
    ldr r2, =_bss_start
    ldr r4, =_bss_end
    movs r3, #0
    b fill_zero_loop

fill_zero:
    str  r3, [r2]       // *(r2) = 0
    adds r2, r2, #4     // r2 += 4

fill_zero_loop:
    cmp r2, r4
    bcc fill_zero       // if(r2 != _bss_end) fill_zero

    /* 调用 libc 的静态初始化 */
    bl __libc_init_array

    /* 调用应用入口点 */
    bl  main
    bx  lr

/* 中断向量表段 */
.section  .vectors, "a"
    .word _estack
    .word reset
```

main.c 

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int aaa = 12345;

void *_sbrk(ptrdiff_t incr)
{
    extern char _end[];
    extern char _heap_end[];
    static char *curbrk = _end;

    if ((curbrk + incr < _end) || (curbrk + incr > _heap_end))
        return NULL - 1;

    curbrk += incr;
    return curbrk - incr;
}

extern void initialise_monitor_handles(void);

int main(int argc, char const *argv[])
{
    initialise_monitor_handles();

    char *data = malloc(0x2000 - 0x100);
    if (data == NULL)
        return -1;

    strcpy(data, "I am Jack");
    printf("Hello World, %s, aaa is %d\n", data, aaa);

    extern uint32_t _data_start;
    extern uint32_t _data_end;
    extern uint32_t _bss_start;
    extern uint32_t _bss_end;
    extern uint32_t _heap_start;
    extern uint32_t _init_data_start;

    printf("_data_start %p\n", &_data_start);
    printf("_data_end %p\n", &_data_end);
    printf("_bss_start %p\n", &_bss_start);
    printf("_bss_end %p\n", &_bss_end);
    printf("_heap_start %p\n", &_heap_start);
    printf("_init_data_start %p\n", &_init_data_start);

    char buf[64];
    scanf("%s", buf);

    printf("%s\n", buf);

    return 0;
}
```

link.ld

```ld
/* 程序入口 */
ENTRY(reset)

/* 栈指针初始位置 */
_estack = ORIGIN(DTCMRAM) + LENGTH(DTCMRAM);

/* 用于预估堆栈大小 */
_heap_size = 0x2000;
_stack_size = 0x4000;

/* 存储器分布 */
MEMORY
{
    DTCMRAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 128K
    FLASH (rx)          : ORIGIN = 0x8000000, LENGTH = 128K
}

/* 段组织 */
SECTIONS
{
    /* 代码 */
    .text :
    {
        KEEP(*(.vectors))
        *(.text)
        *(.text.*)

        KEEP (*(.init))
        KEEP (*(.fini))

    } >FLASH

    /* 只读数据 */
    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
    } >FLASH

    /* 将读写数据段放到 data 段 */
    .data : 
    {
        . = ALIGN(4);
        _data_start = .; 

        *(.data)
        *(.data*)

        . = ALIGN(4);
        _data_end = .;
    } >DTCMRAM AT> FLASH

    /* 指向 flash 中用于初始化的 data */
    _init_data_start = LOADADDR(.data);

    /* 未初始化数据放到 bss 段 */
    .bss :
    {
        . = ALIGN(4);
        _bss_start = .;
        __bss_start__ = _bss_start;
        *(.bss)
        *(.bss*)
        *(COMMON)

        . = ALIGN(4);
        _bss_end = .;
        __bss_end__ = _bss_end;
    } >DTCMRAM

    /* 堆 */
    .heap :
    {
        . = ALIGN(8);
        _heap_start = .;
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + _heap_size;
        _heap_end = .;
        . = . + _stack_size;
        . = ALIGN(8);
    } >DTCMRAM
}
```

Makefile 

```makefile

CC = arm-none-eabi-gcc

LDFLAGS = -mcpu=cortex-m7 -lc -lrdimon -Tlink.ld -Wl,--gc-sections

CFLAGS = -mcpu=cortex-m7 -g 

default:
	@mkdir build -p
	$(CC) -c  $(CFLAGS)   main.c -o build/main.o
	$(CC) -c  $(CFLAGS)   start.s -o build/start.o
	$(CC) build/start.o  build/main.o $(LDFLAGS) -Wl,-Map=build/output.map,--cref -o build/demo.elf

openocd:
	make default && openocd

clean:
	rm build -rf
```

openocd.cfg

```cfg
source [find interface/stlink.cfg]
source [find target/stm32h7x.cfg]

reset_config none separate

program  build/demo.elf verify
```

# 1. 使用 C

## 1.1 初始化栈

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
    /* 代码 */
    .text :
    {
        KEEP(*(.vectors))
        *(.text)
        *(.text.*)

        KEEP (*(.init))
        KEEP (*(.fini))

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

## 1.2 初始化内存变量

虽然这时候可以进入 C 的环境了，但是 C 中用到的内存和变量都还是随机，并没有进行初始化和赋值。这样一来，C 中定义的一些全局变量可能无法正常使用，因此这一步需要在汇编中将变量进行初始化和赋值。

一般来说，为了方便一次性对各个 C 文件中的静态或全局变量进行初始化，在链接器中需要对它们进行合理的安排：

* 首先是只读数据，只读数据在编译阶段就已经确定其值且不会再更改，因此我们可以把只读数据放到 FLASH 中，这样就不需要占用内存了，同时在启动代码中也不需要对其进行赋值和初始化：
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

* 对于有设置了非 0 初值的可读写数据需要同时放到内存以及 FLASH，然后在启动代码中，将 FLASH 中的初值赋值到内存，从而完成变量的初始化。

```ld
/* 将读写数据段放到 data 段 */
.data : 
{
    . = ALIGN(4);
    _data_start = .; 

    *(.data)
    *(.data*)

    . = ALIGN(4);
    _data_end = .;
} >DTCMRAM AT> FLASH

/* 指向 flash 中用于初始化的 data */
_init_data_start = LOADADDR(.data);
```

* 未初始化数据放到 bss 段

```
/* 未初始化数据放到 bss 段 */
.bss :
{
    . = ALIGN(4);
    _bss_start = .;
    __bss_start__ = _bss_start;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _bss_end = .;
    __bss_end__ = _bss_end;
} >DTCMRAM
```

在 ld 中确定好位置后在 start.s 中进行初始化。

```asm
    /* 将 data 段从 flash 拷贝到 SRAM */
    ldr r0, =_data_start
    ldr r1, =_data_end
    ldr r2, =_init_data_start
    movs r3, #0
    b copy_data_loop

copy_data:
    ldr r4, [r2, r3]    // r4 = *(_init_data_start + r3)
    str r4, [r0, r3]    // *(_data_start + r3) = r4
    adds r3, r3, #4     // r3 += 4

copy_data_loop:
    adds r4, r0, r3     // r4 = _data_start + r3
    cmp r4, r1         
    bcc copy_data       // if(r4 != r1) copy_data

    /* 将 0 填充到 .bss 段中 */
    ldr r2, =_bss_start
    ldr r4, =_bss_end
    movs r3, #0
    b fill_zero_loop

fill_zero:
    str  r3, [r2]       // *(r2) = 0
    adds r2, r2, #4     // r2 += 4

fill_zero_loop:
    cmp r2, r4
    bcc fill_zero       // if(r2 != _bss_end) fill_zero
```

# 2. 使用 libc

[参考](https://llvm-gcc-renesas.com/wiki/index.php?title=How_does_Newlib_initialize_the_heap%3F)

arm-none-eabi-gcc 默认使用的是 newlib, 前面已经初始化好 data 和 bss 段，这里只需在 start 中调用 libc 的初始化函数即可 

```
/* 调用 libc 的静态初始化 */
bl __libc_init_array
```

__libc_init_array 函数开始时会调用 *.init 中的段，结束时调用 *.fini 中的段，因此在 .text 中添加

```
KEEP (*(.init))
KEEP (*(.fini))
```

# 3. 使用堆

要使用标准库的 malloc, 需要指定堆内存地址，默认情况下标准库使用 _end 作为堆的起始地址，_end 表示 data 和 bss 结束后的内存地址，即除了 data bss， 剩余的都分给了堆栈

```
/* 堆 */
.heap :
{
    . = ALIGN(8);
    _heap_start = .;
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _heap_size;
    _heap_end = .;
    . = . + _stack_size;
    . = ALIGN(8);
} >DTCMRAM
```

我们把栈放在堆后面，为防止堆栈重叠，或超过可用内存，可预先设置堆栈大小，随着应用变量的使用，堆的初始地址边界会随之向高地址移动，当超过了 RAM 大小，链接时就会报错。

malloc 会调用 _sbrk 函数来确认内存大小，根据链接脚本中的堆地址和大小我们可以实现：
```c
void *_sbrk(ptrdiff_t incr)
{
    extern char _end[];
    extern char _heap_end[];
    static char *curbrk = _end;

    if ((curbrk + incr < _end) || (curbrk + incr > _heap_end))
        return NULL - 1;

    curbrk += incr;
    return curbrk - incr;
}
```

# 4. 使用半主机模式进行调试

要使用半主机，可链接到 `-lrdimon`，librdimon 提供了一系列系统调用函数和 GDB 进行通信，从而实现在调试时执行标准输入输出和文件读写。

半主机模式使用前需进行初始化，使用 initialise_monitor_handles 进行初始化，然后就可以愉快地使用 printf 和 fopen 等函数了。

```c
extern void initialise_monitor_handles(void);

int main(int argc, char const *argv[])
{
    initialise_monitor_handles();

    ...
}
```

当然不要忘记在 GDB 中使能半主机调试，若是用 VSCode 可直接在 setupCommands 中添加

```json
{"text": "monitor arm semihosting enable"}
```

# 5. Map 文件

[参考](https://interrupt.memfault.com/blog/get-the-most-out-of-the-linker-map-file)

map 文件包含了链接的各个 obj 文件的输出情况以及各个段地址的详情信息等，在链接时通过参数 `-Wl,-Map=build/output.map,--cref` 可输出 map 文件。
[参考 2](https://blog.csdn.net/laifengyuan1/article/details/108662522)

`--cref` 可生成交叉引用列表（每个符号出自哪个目标文件，如果还有指定-Map，则会添加到 map 文件中。否则，打印到标准输出）

# 6. Glibc 中的特殊的段

| 段名  | 备注                                                          |
| ----- | ------------------------------------------------------------- |
| .init | 一个函数放到。init 段，在 main 函数执行前系统就会执行它       |
| .fini | 假如一个函数放到。fini 段，在 main 函数返回后该函数就会被执行 |

## __libc_init_array 这个函数中执行的关键过程如下：

调用 .preinit_array 段中的预初始化函数
调用 .init 段中的 _init 函数
调用 .init_array 中的所有函数

这里我需要解释下，在一个段中可能会存在多个函数，根据链接脚本的写法，链接器会在链接时将段名相同的函数指针放到同一个段中，然后通过在段的前后设定锚点来依次执行相应的函数，这样的方式在一些系统的驱动初始化中也能见到！

与 __libc_init_array 对应的函数是 __libc_fini_array，这个函数是在 main 函数执行完成后执行的。它首先会调用 .fini_array 中的所有函数，然后调用 _fini 函数，在嵌入式中一般不会执行到 __libc_fini_array 函数。

在 pc 端也有类似的过程，_init 与 _fini 函数在 crti.o 中被定义。
