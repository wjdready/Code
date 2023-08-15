.syntax unified

/* =========== 代码段 ============= */
.section .text
.globl  reset
.type reset, %function
.thumb

reset:
    nop
    mov r0, #0x23

    bl  test_fun_bl           // 执行 bl 时, cpu 会将返回地址(即函数返回后执行的下一条命令的地址)保存到 lr 寄存器
                              // 因为当前是 Thumb 状态, 为保证返回时仍处于 Thumb，cpu 会自动将 lr + 1
                              // 因此执行后，lr 被自动设为 lr = 下一条语句的地址 + 1

    ldr r0, =test_fun_bx + 1  // 执行无条件转跳 bx 时, CPU会尝试根据目标地址的最低位来确定要切换到的指令集状态, 1 是Thumb 状态，故这里 + 1
    ldr lr, =back + 1         // 但不会保存 lr, 我们可以手动设置, 也要注意 + 1, 否则返回时将换成 ARM 状态了
    bx r0

back:
    ldr r0, =test_fun_bx + 1  // blx 也就说转跳时自动保存 lr 并改变状态, 因为转跳时会改变状态，要注意目标代码处于的状态, 显然这里是 Thumb, 
    blx r0                    // 也需要 + 1变成奇数, 至于返回地址, cpu 会自动根据当前处于的状态自动设置 lr 的 LSB 位, 就不用操心了

    blx test_fun_global_blx   // 这里发现一个奇怪现象, test_fun_global_blx 地址是偶数，而代码是 Thumb 的, 转跳时按理说会发生 
                              // Hardfault 但是，当我设置成 .global 以及 .type 标记为函数后, 竟然就好了, 这违反了前面的知识
                              // 反编译调试后才发现原来是汇编器进行了优化，将 blx 替换成了 bl, 因为编译器会检查会检查到转到的地方根本不是 ARM 状态
                              // 代码, 从而进行优化。 而如果不开全局类型，编译器不知道该处代码，就不会将其优化。它可能会选择保守地将跳转指令保留为 blx
                              // 这时就会发生 Hardfault

    b test_fun                // 最后一个是直接转跳, 没有任何多余的操作 , 这时显然返回时，返回的是上一个函数调用赋值的 lr 对应的地址, 即该行本身
                              // 因此程序最终会在该函数打转

test_fun_bl:
    mov r1, 0x34
    bx lr

test_fun_bx:
    mov r2, 0x34
    bx lr

test_fun_blx:
    mov r3, 0x34
    bx lr

.global test_fun_global_blx
.type test_fun_global_blx, %function
test_fun_global_blx:
    mov r4, 0x34
    bx lr

test_fun:
    mov r5, 0x34
    bx lr

default_handler:
    b .

/* =========== 向量表段 ============= */
.section .isr_vector
    .word _estack
    .word reset                    // 1.  复位
    .word default_handler          // 2.  NMI 不可屏蔽中断 (来自外部 NMI 输入脚)
    .word default_handler          // 3.  Hard fault
    .word default_handler          // 4.  MemManage fault, MPU 访问犯规
    .word default_handler          // 5.  Bus fault
    .word default_handler          // 6.  Usage fault 由编程错误导致的异常
    .word 0                        // 7.  保留
    .word 0                        // 8.  保留
    .word 0                        // 9.  保留
    .word 0                        // 10. 保留
    .word default_handler          // 11. SVC 系统服务调用
    .word default_handler          // 12. 调试监视器 (断点，数据观察点或是外部调试请求)
    .word 0                        // 13. 保留
    .word default_handler          // 14. PendSV 为系统设备而设的 "可悬挂请求"
    .word default_handler          // 15. SysTick 系统滴答定时器
    .word 0                        // 16. 外中断 #0 往下都是外部中断 #0-#239
