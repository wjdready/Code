
.section .text

.global reset
.type reset, %function

reset:
    add r0, r0, #1
    b main

stop:
    b stop

.section .vectors
.code 32

.global system_vectors
system_vectors:
    ldr pc, _vector_reset

_vector_reset:
    .word reset
