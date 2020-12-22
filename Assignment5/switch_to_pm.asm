[bits 16]
; Switch to protected mode
switch_to_pm:

    mov bx, MSG_SWITCHING       ; Log
    call print_string

    cli                         ; Clear interrupts

    lgdt [gdt_descriptor]       ; Load GDT

    mov eax, cr0                ; Set the first bit of cr0 to move to protected mode, cr0 can't be set directly
    or eax, 0x1                 ; Set first bit only
    mov cr0, eax

    jmp CODE_SEG:init_pm        ; Make far jump to to 32 bit code. Forces CPU to clear cache

[bits 32]
; Initialize registers and the stack once in PM
init_pm:

    mov ax, DATA_SEG            ; Now in PM, our old segments are meaningless
    mov ds, ax                  ; so we point our segment registers to the data selector defined GDT
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000            ; Move stack
    mov esp, ebp

    call BEGIN_PM               ; Call 32 bit PM code


; Global variables
MSG_SWITCHING: db "Switching to 32-bit protected mode...", 0