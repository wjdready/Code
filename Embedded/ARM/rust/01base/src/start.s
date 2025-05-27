
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
    @ bl __libc_init_array

    /* 调用应用入口点 */
    bl  main
    bx  lr

/* 中断向量表段 */
.section  .vectors, "a"
    .word _estack
    .word reset
