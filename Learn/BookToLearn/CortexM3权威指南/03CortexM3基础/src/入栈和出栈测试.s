.syntax unified             // 语法统一, 即支持 ARM、Thumb 混合编程

/* ============= 代码段 ================= */
.section .text
.globl  reset               // 必须导出全局, 否则链接文件无法识别
.type reset, %function      // 似乎必须设置 reset 类型为 function 否则无法进入?
reset:
    nop                     // VSCode 第一行无法打断点, 所以这里放了一个空值
    mov r0, #0
    mov r1, #1
    mov r2, #2
    mov r3, #3
    mov r4, #4
    mov r5, #5
    mov r6, #6
    mov r7, #7
    mov r12, #12
    bl test_fun
    bl reset

/* R14 手动入栈 */
test_fun:
    push {r0-r7, r12, r14}
    mov r0, #0x777
    bl test_fun_auto
    // 返回之前出栈
    pop {r0-r7, r12, r14}
    bx r14

/* 使用处理器自动入栈的功能 */
test_fun_auto:
    // 进入程序时，处理器会自动将 R14 入栈了，只要该程序
    // 不调用其他程序，完全不需要再入栈操作内存，提高了程序调用效率
    mov r0, #0x666
    bx lr

/* ============= 向量表段 ================= */
.section .isr_vector
    .word _estack
    .word reset
