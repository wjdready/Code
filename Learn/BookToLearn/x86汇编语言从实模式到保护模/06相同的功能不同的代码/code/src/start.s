.code16
.section .text

_start:

    # 设置段地址
    mov $0xb800, %ax
    mov %ax, %es        # 不能将立即数写入段地址, 所以需要先放到 ax 再放到 ds

    # 写数据到显存
    movb $'-', %es:0    # 默认段地址是 ds, 因此等效于 %ds:0 若使用 es 则需指明 %es:0
    movb $'>', %es:2

    # 将数据段和代码段公用
    mov %cs, %cx
    mov %cx, %ds

    mov $_start, %ax        # ax 是被除数, 也是商放的位置
    mov $10, %si            # 除数
    mov $5, %cx             # 循环次数
    mov $0, %bx

/* cal:
    xor %dx, %dx            # 清空余数
    div %si                 # 除数16位, 执行的是 16 位除法
    add $'0', %dl           # 转成 ascii
    movb %dl, result(%bx)   # 将结果放到内存
    add $2, %bx
    loop cal */

    mov $0xb800, %ax
    mov %ax, %es

    mov $str, %si           # 源地址 ds:si
    mov $0, %di             # 目的地址 es:di
    mov $str_len, %cx    # 数量
    rep movsw

here:
    jmp here

str:
    .byte 'H', 4, 'e', 4, 'l', 4, 'l', 4, 'o', 4, ',', 4
    .byte 'W', 4, 'o', 4, 'r', 4, 'l', 4, 'd', 4, '!', 4
str_len = (. - str) / 2

    .fill 510 - (. - _start), 1, 0
    .word 0xaa55
