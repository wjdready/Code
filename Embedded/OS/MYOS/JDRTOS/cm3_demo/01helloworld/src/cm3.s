.syntax unified

.section .text 
.globl reset
.type reset, %function

reset:
    nop
    mov r0, #1
    bl init_bss_section
    bl init_data_section
    bl main
    b .
