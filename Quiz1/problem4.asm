global long_add
global extended_long
long_add:
    mov eax, edi
    add eax, esi
    add eax,edx
    add eax, ecx
    ret
extended_long:
    mov rax, rdi
    add rax, rsi
    add rax, rdx
    add rax, rcx
    ret