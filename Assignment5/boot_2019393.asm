[bits 16]
[org 0x7c00]                    ; bootloader's start address is at this location

xor ax, ax                      ; cleaning ax register 
mov ss, ax                      ; setting ss to 0
mov bp, 0x9000                  ; taking base pointer to a particular location
mov sp, 0x9000                  ; taking stack pointer to the same location as base poiter

;call switch_to_pm

switch_to_protected_mode:
    cli                         ; clear all interrupts
    lgdt [gdt_descriptor]       ; load global descriptor table
    mov eax, cr0                ; need to change the last bit of cr0 to shift to protected mode
    or eax, 0x0001
    mov cr0, eax
    jmp CE:protected_mode_code



gdt_start:                      ; not required in 16 bit mode
    gdt_null:   
        dd 0x0                  ; dd = allocate memory to a particular address and put in the given data
        dd 0x0                  ; done twice to get 8 bytes of 0 for null segment of the register
    gdt_code:   
        dw 0xffff               ; dd = 4 bytes
        dw 0x0                  ; dw = 2 bytes
        db 0x0                  ; db = 1 byte
        db 10011010b
        db 11001111b
        db 0x0
    gdt_data:
        dw 0xffff
        dw 0x0
        db 0x0
        db 10010010b
        db 11001111b
        db 0x0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1 
    dd gdt_start

CE equ gdt_code - gdt_start   

DA equ gdt_data - gdt_start



[bits 32]

print_register:
    pusha
    mov edx, cr0
    mov ecx, 32
    mov ebx, 000B8000h
    add ebx, 1330
    loop:                           ; printing content of cr0
        mov eax, 00000130h          ; prints blue color 
        shl edx, 1                  ; shift left and checking value
        adc eax, 0                  ; add with carry 
        mov [ebx], ax   
        add ebx, 2                  ; move 2 forward to avoid overlapping 
        dec ecx                     ; decrease ecx by 1 -> only print 32 bits
        jnz loop                    ; if ecx not 0 , jump to loop again 
    popa                            ; pop all from stack
    ret

print_hello_world:
    pusha                           ; pushes all address to stack
    mov ebx, hello_message
    mov edx, 000B8000h              ; display's start of the screen's address
    add edx, 1280
    mov eax, 00000130h
    loops:
        mov al, [ebx]               ; 16 bit reg = al
        cmp al, 0
        je pm_return
        mov [edx], ax
        add ebx, 1
        add edx, 2
        jmp loops

init_seg_registers:
    mov ds, ax                     ; _s are segment registers
    mov ss, ax                     ; replace their value by value of ax register
    mov es, ax
    mov fs, ax
    mov gs, ax    
    ret

protected_mode_code:
    mov ax, DA
    
    call init_seg_registers

    mov ebp, 0x9000                 ; setting base pointer
    mov esp, 0x9000                 ; setting stack pointer

    call print_hello_world
    call print_register
    jmp $                           ; $ holds the address of current line 


pm_return:
    popa                            ; pop all 
    ret


hello_message: db "Hello world   Value: ", 0

times 510-($-$$) db 0

dw 0xaa55