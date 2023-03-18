
.syntax unified

/* 代码段 */
.section .text.reset
.type reset, %function
.globl  reset

/* 程序入口 */
reset:

    ldr sp, =_estack

     /* Copy the data segment initializers from flash to SRAM */
    ldr r0, =_sdata
    ldr r1, =_edata
    ldr r2, =_sidata
    movs r3, #0
    b LoopCopyDataInit

CopyDataInit:
    ldr r4, [r2, r3]
    str r4, [r0, r3]
    adds r3, r3, #4

LoopCopyDataInit:
    adds r4, r0, r3
    cmp r4, r1
    bcc CopyDataInit

    /* 将 0 填充到 .bss 段中 */
    ldr r2, =_sbss
    ldr r4, =_ebss
    movs r3, #0
    b LoopFillZerobss

FillZerobss:
    str  r3, [r2]
    adds r2, r2, #4

LoopFillZerobss:
    cmp r2, r4
    bcc FillZerobss

    /* 调用 libc 的静态初始化 */
    @ bl __libc_init_array

    /* 调用应用入口点 */
    bl  main
    bx  lr

/* 中断向量表段 */
.section  .vectors, "a"
    .word _estack
    .word reset
