.syntax unified             // 语法统一, 即支持 ARM、Thumb 混合编程

/* =========== 代码段 ============= */
.section .text
.globl  reset               // 必须导出全局, 否则链接文件无法识别
.type reset, %function      // 似乎必须设置 reset 类型为 function 否则无法进入?

reset:
    nop                    // 向量表大小8字节 + nop指令2字节 = 0xa, 因此下一条指令位置是 0x0a
    mov r0, pc             // 由于使用了流水线，获取 pc 值时，pc 实际指向下一条语句了, 因此这时 r0 的值 = 当前代码地址 + 4 = 0xe

    // 计算 demo 的实际内存地址
    mov r0, demo            // 这里 demo 的值并不是 demo 对应的实际内存地址，而是相对整个代码段的偏移地址 
    add r0, 8               // 在偏移地址的基础上 + 向量表 8 字节，从而得到 demo 的实际地址
    add r0, 1               // 确保 pc 地址是奇数, 保持处于 Thumb 状态

    // 计算返回地址, 直接操作 pc 进行转跳硬件不会帮我们备份用于返回的地址, 这里手动计算
    mov r14, 1              // 返回时也要确保 pc 地址是奇数, 保持处于 Thumb 状态
    add r14, pc             // 这里刚好后两条语句都是 2 字节的, 因此返回地址就是 b reset 的地址

    mov pc, r0              // 直接赋值 pc 转跳到 demo
    b reset

demo:
    mov r1, #0x56
    bx lr

/* =========== 向量表段 ============= */
.section .isr_vector
    .word _estack
    .word reset
