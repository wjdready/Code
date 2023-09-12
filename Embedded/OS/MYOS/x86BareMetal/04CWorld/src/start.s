    .code16
    .section .text

INIT_SS_REG = 0x9000
INIT_SP_REG = 0xff00

_start:
    
    mov $INIT_SS_REG, %ax
    mov %ax, %ss

    mov $INIT_SP_REG, %ax
    mov %ax, %sp

    # 设置完堆栈后直接进入 C 的世界
    call main

.section .boot_end,"a"
    .word 0xaa55
