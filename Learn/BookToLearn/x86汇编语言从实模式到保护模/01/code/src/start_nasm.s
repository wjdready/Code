bits 16

section .text
    mov ax, cs
    mov ds, ax
    mov es, ax
    call disp_str

disp_str:
    mov ax, boot_message
    mov bp, ax
    mov cx, 16
    mov ax, 01301h
    mov bx, 000ch
    mov dl, 0
    int 10h     ; 10 号中断

    ret

boot_message:   db "Hello, OS world!"
times 510-($-$$) db 0 ; 填充剩余空间
dw 0xaa55             ; 结束标志
