    .code16
    .section .text

_start:
    mov $0xb800, %ax
    mov %ax, %es

    # 显示字符串
    mov $str_len, %cx       # 要显示字符串数量
    mov $0, %si             # 用于字符串地址索引
    mov $0, %di             # 用于显存地址索引
    mov $0x03, %ah          # 每个字符的显示属性

disp:
    movb str(%si), %al      # 获取当前索引字符
    mov %ax, %es:(%di)      # 复制到显存, 源操作数 ax, 复制的是 16 位
    add $2, %di             # 一次复制2个字节, 包括显示属性字节, 显存索引 + 2  
    inc %si                 # 字符串索引 + 1
    loop disp               # 重复执行直到 cx = 0

    jmp .

str:
    .ascii "Hello, World! AS YOU CAN SEE, So good!"
    str_len = (. - str)

    .fill 510 - (. - _start), 1, 0
    .word 0xaa55
