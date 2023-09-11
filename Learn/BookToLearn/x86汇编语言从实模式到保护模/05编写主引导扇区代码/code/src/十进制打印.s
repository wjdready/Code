.code16
.section .text

_start:

    # 设置段地址
    mov $0xb800, %ax
    mov %ax, %es    # 不能将立即数写入段地址, 所以需要先放到 ax 再放到 ds

    # 写数据到显存
    movb $'-', %es:0    # 默认段地址是 ds, 因此等效于 %ds:0 若使用 es 则需指明 %es:0
    movb $'>', %es:2

    # 将数据段和代码段公用
    mov %cs, %cx
    mov %cx, %ds

lab:
    mov $lab, %ax        # ax 是被除数, 也是商放的位置
    mov $10, %bx         # 除数

    xor %dx, %dx         # 清空余数
    div %bx              # 除数16位, 执行的是 16 位除法
    movb %dl, number + 0 # 将结果放到内存
    
    xor %dx, %dx
    div %bx
    movb %dl, number + 1

    xor %dx, %dx
    div %bx
    movb %dl, number + 2
    
    xor %dx, %dx
    div %bx
    movb %dl, number + 3

    xor %dx, %dx
    div %bx
    movb %dl, number + 4

    # 从内存中依次取出数据并打印出来
    mov number + 4, %al
    add $'0', %al
    movb %al, %es:4

    mov number + 3, %al
    add $'0', %al
    movb %al, %es:6

    mov number + 2, %al
    add $'0', %al
    movb %al, %es:8

    mov number + 1, %al
    add $'0', %al
    movb %al, %es:10

    mov number + 0, %al
    add $'0', %al
    movb %al, %es:12

here:
    jmp here

number:
    .space 10

    .fill 510 - (. - _start), 1, 0
    .word 0xaa55
