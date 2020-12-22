;In 16 bit mode
[bits 16]
[org 0x7c00]

xor ax, ax
mov ss, ax
mov bp, 0x9000
mov sp, 0x9000

call switch_to_pm

gdt_start:
    gdt_null:
        dd 0x0, 0x0
    gdt_code:
        dw 0xffff, 0x0
        db 0x0, 10011010b, 11001111b, 0x0
    gdt_data:
        dw 0xffff, 0x0
        db 0x0,10010010b, 11001111b, 0x0
gdt_end
gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start
CODE equ gdt_code - gdt_start
DATA equ gdt_data - gdt_start


switch_to_pm:
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 0x0001
    mov cr0, eax
    jmp CODE:pm_code


[bits 32]
pm_code:
    mov ax, DATA
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x9000
    mov esp, 0x9000

    call print_hw
    call print_register
    jmp $

print_hw:
    pusha
    mov ebx, MSG_P
    mov edx, 000B8000h
    add edx, 1280
    loops:
        mov al, [ebx]
        cmp al, 0
        je pm_return
        mov [edx], ax
        add ebx, 1
        add edx, 2
        jmp loops

pm_return:
    popa
    ret

print_register:
    pusha
    mov edx, cr0
    mov ecx, 32
    mov ebx, 000B8000h
    add ebx, 1330
    loop:
        mov eax, 00000130h
        shl edx, 1
        adc eax, 0
        mov [ebx], ax
        add ebx, 2
        dec ecx
        jnz loop
    popa
    ret

MSG_P: db "Hello world    CR0 Value: ", 0

times 510-($-$$) db 0
dw 0xaa55