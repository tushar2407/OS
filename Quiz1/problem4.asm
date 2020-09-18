global long_add
global extended_long
long_add:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    add eax, [ebp+12]
    add eax, [ebp+16]
    add eax, [ebp+20]
    mov esp,ebp
    pop ebp
    ret