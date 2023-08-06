.syntax unified             // 语法统一, 即支持 ARM、Thumb 混合编程

/* =========== 代码段 ============= */
.section .text
.globl  reset               // 必须导出全局, 否则链接文件无法识别
.type reset, %function      // 似乎必须设置 reset 类型为 function 否则无法进入?

reset:
    nop                     // VSCode 第一行无法打断点, 所以这里放了一个空值
    mov r0, #0x23
    bl .

/* =========== 向量表段 ============= */
.section .isr_vector
    .word _estack           // cpu 复位后会取向量表第一个 word 赋值给 sp
    .word reset             // 并取第二个 word 赋值给 pc, 从而转跳到 reset 代码
    // 我们的链接文件中向量表段后紧接着是 .text 段，因此 reset 实际地址就是向量表大小, 从而 pc 默认初值就是向量表大小
    // 当然这取决于 reset 刚好被链接到地址紧接着向量表段, 因为这里就一个 .s 文件且 reset 在源文件中处于最开始位置
    // 实际上如果在 reset 之前包含其他 .text 代码部分也不奇怪, 这时 pc 指针虽然还是 reset 地址, 但是就不是向量表大小
