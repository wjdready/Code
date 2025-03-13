.code16
.section .text
.global loadbin

INIT_SS_REG = 0x9000
INIT_SP_REG = 0xff00

_start:
    mov $0xb800, %ax
    mov %ax, %es

    mov $INIT_SS_REG, %ax
    mov %ax, %ss

    mov $INIT_SP_REG, %ax
    mov %ax, %sp

    # 设置完堆栈后直接进入 C 的世界
    call main

loadbin:

    # 设置驱动器号，读取第一个扇区
    movb $0x02, %ah         # 功能码：读取扇区
    movb 4(%esp), %cl       # 扇区号 (第一个参数)
    movb 8(%esp), %al       # 读取 n 个扇区 (第二个参数)
    movb $0, %ch            # 气缸号
    movb $0, %dh            # 磁头号
    movb $0x80, %dl         # 驱动器号（0x80 表示第一个硬盘）

    # 设置内存地址
    movw 12(%esp), %bx       # 数据存储的缓冲区地址
    movw %bx, %es            # 将 ES 设置为缓冲区的段地址
    movw 16(%esp), %bx       # BX 指向缓冲区的偏移

    # 调用中断
    int $0x13

    ret

# display_char 函数
# 参数：偏移地址在 %rdi 中，字符在 %esi 中
# display_char:

#    # 设置段寄存器
#    movl $0xb800, %eax      # 将显存段地址加载到 EAX
#    movw %ax, %es           # 将 AX 复制到 ES

#    # 获取参数
#    movw 4(%esp), %di       # 获取偏移地址 (offset)，在 %esp + 4
#    movb 8(%esp), %al        # 获取字符 (c)，在 %esp
#    # 显示字符
#    mov %al, %es:(%di)      # 将字符存储到显存偏移地址
#    add $1, %di

#    # 颜色
#    movb 12(%esp), %al        # 获取字符 (c)，在 %esp
#    # movb $0x03, %al         # 设置字符属性（白色字符，黑色背景）
#    movb %al, %es:(%di)       # 将属性存储到显存偏移地址 + 1
#    ret                       # 返回到调用者

.section .boot_end,"a"
.word 0xaa55
