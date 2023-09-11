    .code16
    .section .text

_start:
    mov $0xb800, %ax
    mov %ax, %es

    mov $str, %si           # 源地址 ds:si
    mov $0, %di             # 目的地址 es:di
    mov $str_len, %cx       # 数量
    rep movsw

    jmp .

str:
    .byte 'H', 4, 'e', 4, 'l', 4, 'l', 4, 'o', 4, ',', 4
    .byte 'W', 4, 'o', 4, 'r', 4, 'l', 4, 'd', 4, '!', 4
    str_len = (. - str) / 2

    .fill 510 - (. - _start), 1, 0
    .word 0xaa55
