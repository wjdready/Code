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
    b main

/* 中断向量表段 */ 
.section  .isr_vector, "a"
    .word _estack
    .word reset
