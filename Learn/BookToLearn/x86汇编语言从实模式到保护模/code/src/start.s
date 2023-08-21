.code16
.section .text

_start:
    mov %cs, %ax
    mov %ax, %ds
    mov %ax, %es
    call disp_str

disp_str:
    mov $boot_message, %eax
    mov %ax, %bp
    mov $16, %cx
    mov $0x1301, %ax
    mov $0x0c, %bx
    mov $0, %dl
    int $0x10

    ret

boot_message:
    .ascii "Hello, OS world!"

    .fill 510 - (. - _start), 1, 0
    .word 0xaa55
