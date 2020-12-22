print_string:
    pusha
    mov ah, 0x0e

_print_str_loop:
    mov al, [bx]
    cmp al, 0
    je _print_str_return
    int 0x10
    inc bx
    jmp _print_str_loop

_print_str_return:
    popa
    ret