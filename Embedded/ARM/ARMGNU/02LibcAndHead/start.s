/* 代码段 */
.section .text
.type reset, %function
.globl  reset

/* 程序入口 */
reset:

    bl __libc_init_array
    
    b main

/* 中断向量表段 */
.section  .isr_vector, "a"
    .word _estack
    .word reset
