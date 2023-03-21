#### ARM GNU 编译与链接01: 工程创建, 程序烧写和调试

基于 STM32 或 QEMU 平台, 对编译与链接原理进行探究，以及学习 ARM 汇编指令集, GNU 的汇编语法。

### 从零开始的工程创建

这里以一个简单的汇编程序为例

#### 1. 源代码 start.s

```s
/* 代码段 */
.section .text
.type reset, %function
.globl  reset

/* 程序入口 */
reset:
    mov r0, #0x66
    mov r1, #255

    push {r0}
    mov r0, r1
    pop {r0}
    b .

/* 中断向量表段 */
.section  .isr_vector, "a"
    .word _estack
    .word reset
```

#### 2. 链接文件：link.ld
```ld
/* 程序入口 */
ENTRY(reset)

/* 栈指针初始位置 */
_estack = 0x20020000;

/* 存储器分布 */
MEMORY
{
    DTCMRAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 128K
    FLASH (rx)          : ORIGIN = 0x8000000, LENGTH = 128K
}

/* 段组织 */
SECTIONS
{
    /* FLASH 起始地址放置中断向量表 */
    .my_vertor :
    {
        /* 中断向量表的数据一般是由CPU中断自动调用的, 在程序中有可能没被引用, 
        容易被GCC的优化程序当成垃圾回收, 因此使用 KEEP 保持原样不变 */
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } >FLASH

    /* 代码段紧随其后 */
    .text :
    {
        . = ALIGN(4);
        *(.text)
        . = ALIGN(4);
    } >FLASH
}
```

> 上述定义了两个段, 分别是代码段和中断向量表段。在 STM32 中 FLASH的起始地址为0x08000000, 可以通过设置BOOT0引脚来设置程序从FLASH启动, 这时复位后 MCU 在执行完内固化  程序后将会从0x08000000 地址加载 sp 指针, 并从 0x08000004 加载 pc 指针并转跳。
> 因此上面定义中断向量表段, 包含了栈指针初值和入口地址, 这个栈指针初值其实就是RAM内存的末 尾, 因为 ARM 的入栈时sp指针自减, 放在末尾保证最开始初始化时有足够大的栈空间。


#### 3. 编译文件 Makefile
```makefile
CC = arm-none-eabi-gcc

LDFLAGS = -mcpu=cortex-m7 -Tlink.ld -Wl,--gc-sections

CFLAGS = -mcpu=cortex-m7 -g

default:
	@mkdir build -p
	$(CC) -c  $(CFLAGS)   start.s -o build/start.o
	$(CC) build/start.o  $(LDFLAGS) -o build/demo.elf
```

#### 4. 下载和调试文件 openocd.cfg

```cfg
# 设置调试器和MCU, openocd 已经给我们写好了
source [find interface/stlink.cfg]
source [find target/stm32h7x.cfg]

# 复位设置, 对于 ST-link SWD 模式, 必须设置才能软复位
reset_config none separate

# 下载程序
program  build/demo.hex verify
```

#### 5. 编译和调试

```
$ ls
link.ld  Makefile  openocd.cfg  start.s
$ make
arm-none-eabi-gcc -x assembler-with-cpp -c -mcpu=cortex-m7  -g -gdwarf-2 -Og -Wall -fdata-sections -ffunction-sections   start.s -o build/start.o
arm-none-eabi-gcc build/start.o   -specs=nano.specs -Tlink.ld -lc -lm -lnosys -Wl,--gc-sections -o build/demo.elf
```

下载和开启调试服务

```
$ openocd
...
** Programming Started **
...
** Programming Finished **
** Verify Started **
** Verified OK **
...
Info : Listening on port 6666 for tcl connections
Info : Listening on port 4444 for telnet connections
```

使用GDB进行调试:

```
$ arm-none-eabi-gdb build/demo.elf
...
Reading symbols from build/demo.elf...
(gdb) target remote localhost:3333
reset () at start.s:10
10          mov r0, #0x66
(gdb) n
halted: PC: 0x0800000a
11          mov r1, #255
```

#### 使用 VSCode
使用 VSCode 进行调试需要设置 launch.json
```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "(gdb) 启动",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/build/demo.elf",
            "args": [],
            "stopAtEntry": true,
            "cwd": "${fileDirname}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "miDebuggerPath": "arm-none-eabi-gdb",
            "setupCommands": [
                    {"text": "set remotetimeout 5"},
                    {"text": "target extended-remote localhost:3333"},
                    {"text": "monitor reset halt"},
            ]
        }
    ]
}
```

确保使用 openocd 打开调试服务, 然后开始调试如图所示
![image.png](https://upload-images.jianshu.io/upload_images/17837162-a2a162e93108d452.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

> 发现 VSCode 没能在汇编上打断点, 不过可以在左边栏手动添加断点标签, 比如上面在reset标签打了断点, 所以程序就在入口出停了下来, 这时观察左边栏的寄存器信息, 发现 sp指针正是 0x20020000, pc 指针是 0x8000008, 因为中断向量表只放了2 个word数据, 也就是8字节, 在 link.ld 中 中断向量表后面立刻是程序了, 所以程序复位后pc值就是0x8000008。
> 同时我们可以在下面的输出栏的调试控制台对GDB发送命令来查看更多信息, 比如打印FLASH初始地址的内容 `-exec x /4x 0x8000000`, 查看某个程序的汇编内容 `-exec disassemble reset` 
> 成功进入调试后, 单步运行查看寄存器和pc、sp指针的变化, 理解这个过程


### 编译器选项说明


作为入门, Makefile 尽可能地简单明了, 上述 Makefile 只列出了关键的编译选项:

#### 1. -mcpu=cortex-m7

这是编译和链接过程都都需要的给出的, 该选项指出了 MCU 的核心类型为 cortex-m7, 这是必要的, 因为编译器默认会编译出 cortex-a 指令集, 可能无法运行在 MCU 中

#### 2. -Tlink.ld

该选项向链接器说明使用我们自己的链接脚本, 如不指明, 则编译器使用默认的内置链接脚本, 其结果可能不是我们想要的.

#### 3. -Wl,--gc-sections 

-Wl 开头表示要将后面的参数传递给链接器ld（多个选项的传递通过逗号进行分隔）, 这里 `--gc-sections` 的作用是使链接器删除没有被使用的 section. 我们的链接脚本中只使用了 .my_vertor 和 .text section, 而编译器在编译的时候会自动加入一些段, 使用 `--gc-sections` 将它们删除.

#### 参考文献

* 这篇博客详细介绍一些 [GCC 编译选项和作用](https://blog.csdn.net/laifengyuan1/article/details/108662522?utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_baidulandingword~default-1-108662522-blog-118416568.pc_relevant_aa&spm=1001.2101.3001.4242.2&utm_relevant_index=4)

* 这个人的博客有很多关于 [ARM 架构文章](https://www.jianshu.com/p/e7705a8aa03a) , 值得一看

* 该博主给出了一些列构建[树莓派4系统](https://jsandler18.github.io/explanations/linker_ld.html) 的文章, 有很多参考意义

[关于编译器会自然默认链接标准库里的内容](https://stackoverflow.com/questions/57357726/linking-failed-when-use-indirectly-the-gnu-linker)
