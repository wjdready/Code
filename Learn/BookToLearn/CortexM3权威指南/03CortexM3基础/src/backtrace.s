.syntax unified

/* =========== 代码段 ============= */
.section .text
.globl  reset
.type reset, %function

reset:
    nop
    mov r0, #0x23
    bl  f1          // 执行 bl 时, cpu 会将返回地址(即函数返回后执行的下一条命令的地址)保存到 lr 寄存器
    b   reset       // 在 Thumb 模式, lr 的值为了保持奇数，会自动 + 1

f1:
    push {lr}       // 因而这里的 lr 是 b reset 所在地址 (Thumb 下 lr 还要 -1), 下面同理的
    mov r1, 0x34
    bl f2
    pop {lr}
    bx lr

f2:
    push {lr}
    mov r2, 0x34
    bl f3
    pop {lr}
    bx lr

f3:
    push {lr}       // 随着调用的的进行, sp 指向的内存区域保存着历史调用时备份的 lr (即返回地址)
    mov r3, 0x34    // 这时通过遍历栈内存, 每次读一个 word, 获取可能是返回地址的数据 (当然也可能是其他数据)
    bl f4           // 我们每次都假设读到的数据是返回地址，然后基于这个地址 (Thumb 下还要 -1)， 往前读一个指令
    pop {lr}        // 若前一个指令是 BL 或 BLX, 就可以确定这个地址就是我们要找的返回地址
    bx lr

f4:
    mov r4, 0x34
    udf #1 // 触发一个未定义指令
    bx lr

default_handler:
    b .

hardfault_handler:
    mov r0, lr
    mov r1, sp
    bl backtrace
    b .

/* =========== 向量表段 ============= */
.section .isr_vector
    .word _estack
    .word reset                    // 1.  复位
    .word default_handler          // 2.  NMI 不可屏蔽中断 (来自外部 NMI 输入脚)
    .word hardfault_handler + 1    // 3.  Hard fault
    .word default_handler          // 4.  MemManage fault, MPU 访问犯规
    .word default_handler          // 5.  Bus fault
    .word default_handler          // 6.  Usage fault 由编程错误导致的异常
    .word 0                        // 7.  保留
    .word 0                        // 8.  保留
    .word 0                        // 9.  保留
    .word 0                        // 10. 保留
    .word default_handler + 1      // 11. SVC 系统服务调用
    .word default_handler          // 12. 调试监视器 (断点，数据观察点或是外部调试请求)
    .word 0                        // 13. 保留
    .word default_handler          // 14. PendSV 为系统设备而设的 "可悬挂请求"
    .word default_handler          // 15. SysTick 系统滴答定时器
    .word 0                        // 16. 外中断 #0 往下都是外部中断 #0-#239
