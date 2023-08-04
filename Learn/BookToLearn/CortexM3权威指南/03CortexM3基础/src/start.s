
/* ============= 代码段 ================= */
.section .text

.globl  reset
.type reset, %function
reset:
    nop
gaga:
    ldr r0, =reset
fule:
    ldr r1, =gaga
    ldr r0, =fule
    push {r0}
    push {r0}
    push {r0}
    pop {r2}  
    BL reset

/* ============= 向量表段 ================= */
.section .isr_vector
    .word _estack
    .word reset
