; Boot sector that enters 32 bit protected mode
[org 0x7c00]

mov bp, 0x9000          ; Set stack
mov sp, bp

mov bx, MSG_REAL_MODE
call print_string

call switch_to_pm       ; We will never return to here

jmp $

%include "print_string.asm"
%include "gdt.asm"
%include "print_string_pm.asm"
%include "switch_to_pm.asm"

[bits 32]
;Where we arrive after switching to PM
BEGIN_PM:
    mov ebx, MSG_PROTECTED_MODE
    call print_string_pm        ; 32 bit routine to print string

    jmp $                       ; Hang


; Global variables
MSG_REAL_MODE: db "Started in 16-bit real mode.", 0
MSG_PROTECTED_MODE: db "Successfully landed in 32-bit protected mode.", 0

; Boot sector padding
times 510-($-$$) db 0
dw 0xaa55