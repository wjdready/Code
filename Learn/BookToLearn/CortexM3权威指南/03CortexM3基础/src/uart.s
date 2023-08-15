.syntax unified

/* =========== 代码段 ============= */
.section .text
.globl  reset
.type reset, %function

reset:
    nop
    blx main
    bl .

/* =========== 向量表段 ============= */
.section .isr_vector
    .word _estack
    .word reset
