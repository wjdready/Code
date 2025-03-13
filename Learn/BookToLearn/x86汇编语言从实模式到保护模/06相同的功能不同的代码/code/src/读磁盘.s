.code16
.section .text

_start:
    # 设置驱动器号，读取第一个扇区
    movb $0x02, %ah         # 功能码：读取扇区
    movb $1, %al            # 读取 1 个扇区
    movb $0, %ch            # 气缸号
    movb $2, %cl            # 扇区号（假设从第 2 扇区开始）
    movb $0, %dh            # 磁头号
    movb $0x80, %dl         # 驱动器号（0x80 表示第一个硬盘）

    # 设置内存地址
    movw $0x9000, %bx       # 数据存储的缓冲区地址
    movw %bx, %es           # 将 ES 设置为缓冲区的段地址
    xor %bx, %bx            # BX 清零，指向缓冲区的偏移

    # 调用中断
    int $0x13

    jmp .

.section .boot_end,"a"
.word 0xaa55
