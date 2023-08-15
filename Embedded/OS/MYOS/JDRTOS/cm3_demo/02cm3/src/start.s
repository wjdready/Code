.syntax unified

/* =========== 代码段 ============= */
.section .text 
.globl reset
.type reset, %function

reset:
    nop
    mrs r0, CONTROL
    mov r1, #2
    orr r0, r1
    msr CONTROL, r0      // 配置 CONTROL[1] = 1, 使用 PSP 指针, MSP 默认被 cpu 自动赋值为向量表第一个 word 了

    ldr r0, =_estack_psp // 切换为 PSP 后, 赋值
    mov sp, r0

    bl init_bss_section
    bl init_data_section
    bl main
    b .

.globl get_psp
get_psp:
    mrs r0, PSP
    bx lr

.globl get_msp
get_msp:
    mrs r0, MSP
    bx lr

.globl get_control_reg
get_control_reg:
    mrs r0, CONTROL
    bx lr

default_handler:
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
    .word systick_handler + 1      // 15. SysTick 系统滴答定时器
    .word 0                        // 16. 外中断 #0 往下都是外部中断 #0-#239
