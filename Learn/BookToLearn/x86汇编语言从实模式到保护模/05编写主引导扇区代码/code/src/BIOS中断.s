    .code16
    .section .text

_start:
    /* 
        中断调用号：0x10
        寄存器ax：功能调用（ah:功能号，al:子功能号）
            如0x1301 表示功能号19，子功能01（写字符串并移动光标到串结尾处）。
        寄存器bx：字符属性
        寄存器cx：字符串长度
        寄存器dx：显示位置(dh:行，dl:列)
        起存期es:bp：字符串地址
    */

    mov $boot_message, %bp     # es:bp 字符串地址
    mov $0x1301, %ax           # 功能码
    mov $0x0c, %bx             # 字符属性
    mov $16, %cx               # 字符串长度
    mov $0, %dx                # 显示位置 (dh:行，dl:列)
    int $0x10

    ret

boot_message:
    .ascii "Hello, OS world!"

    .fill 510 - (. - _start), 1, 0
    .word 0xaa55
