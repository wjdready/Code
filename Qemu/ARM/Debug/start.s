/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2011-01-13     weety      first version
 * 2015-04-15     ArdaFu     Split from AT91SAM9260 BSP
 * 2015-04-21     ArdaFu     Remove remap code. Using mmu to map vector table
 * 2015-06-04     aozima     Align stack address to 8 byte.
 */

.equ MODE_USR,        0x10
.equ MODE_FIQ,        0x11
.equ MODE_IRQ,        0x12
.equ MODE_SVC,        0x13
.equ MODE_ABT,        0x17
.equ MODE_UND,        0x1B
.equ MODE_SYS,        0x1F
.equ MODEMASK,        0x1F
.equ NOINT,           0xC0

.equ I_BIT,           0x80
.equ F_BIT,           0x40

.equ UND_STACK_SIZE,  0x00000100
.equ SVC_STACK_SIZE,  0x00000100
.equ ABT_STACK_SIZE,  0x00000100
.equ FIQ_STACK_SIZE,  0x00000100
.equ IRQ_STACK_SIZE,  0x00000100
.equ SYS_STACK_SIZE,  0x00000100

 /*
 ***************************************
 * Interrupt vector table
 ***************************************
 */
.section .vectors
.code 32

.global system_vectors
system_vectors:
    ldr pc, _vector_reset
    ldr pc, _vector_undef
    ldr pc, _vector_swi
    ldr pc, _vector_pabt
    ldr pc, _vector_dabt
    ldr pc, _vector_resv
    ldr pc, _vector_irq
    ldr pc, _vector_fiq

_vector_reset:
    .word reset
_vector_undef:
    .word vector_undef
_vector_swi:
    .word vector_swi
_vector_pabt:
    .word vector_pabt
_vector_dabt:
    .word vector_dabt
_vector_resv:
    .word vector_resv
_vector_irq:
    .word vector_irq
_vector_fiq:
    .word vector_fiq

.balignl    16,0xdeadbeef

 /*
 ***************************************
 *  Stack and Heap Definitions
 ***************************************
 */
    .section .data
    .space UND_STACK_SIZE
    .align 3
    .global und_stack_start
und_stack_start:

    .space ABT_STACK_SIZE
    .align 3
    .global abt_stack_start
abt_stack_start:

    .space FIQ_STACK_SIZE
    .align 3
    .global fiq_stack_start
fiq_stack_start:

    .space IRQ_STACK_SIZE
    .align 3
    .global irq_stack_start
irq_stack_start:

    .skip SYS_STACK_SIZE
    .align 3
    .global sys_stack_start
sys_stack_start:

    .space SVC_STACK_SIZE
    .align 3
    .global svc_stack_start
svc_stack_start:

/*
 ***************************************
 * Startup Code
 ***************************************
 */
    .section .text
    .global reset
reset:
    /* Enter svc mode and mask interrupts */
    mrs r0, cpsr
    bic r0, r0, #MODEMASK
    orr r0, r0, #MODE_SVC|NOINT
    msr cpsr_cxsf, r0

    /* init cpu  */
    bl  cpu_init_crit

    b main
    /* Call low level init function */
    ldr     sp, =svc_stack_start
    @ ldr     r0, =rt_low_level_init
    blx     r0

    /* init stack */
    @ bl stack_setup

    /* clear bss */
    mov     r0, #0
    @ ldr     r1, =__bss_start
    @ ldr     r2, =__bss_end

bss_clear_loop:
    cmp     r1, r2
    strlo   r0, [r1], #4
    blo     bss_clear_loop

    /* call c++ constructors of global objects */
    /*
    ldr     r0, =__ctors_start__
    ldr     r1, =__ctors_end__

ctor_loop:
    cmp     r0, r1
    beq     ctor_end
    ldr     r2, [r0], #4
    stmfd   sp!, {r0-r1}
    mov     lr, pc
    bx      r2
    ldmfd   sp!, {r0-r1}
    b       ctor_loop
ctor_end:
    */
    /* start RT-Thread Kernel */
@     ldr     pc, _rtthread_startup
@ _rtthread_startup:
@     .word  rtthread_startup



cpu_init_crit:
    /* invalidate I/D caches */
    mov r0, #0
    mcr p15, 0, r0, c7, c7, 0
    mcr p15, 0, r0, c8, c7, 0

    /* disable MMU stuff and caches */
    mrc p15, 0, r0, c1, c0, 0
    bic r0, r0, #0x00002300
    bic r0, r0, #0x00000087
    orr r0, r0, #0x00000002
    orr r0, r0, #0x00001000
    mcr p15, 0, r0, c1, c0, 0

    bx lr
