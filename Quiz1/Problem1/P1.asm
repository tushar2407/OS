global _start
global print1
global print0
global checkGreater
section .text
_start: 
    mov rdi, 5
    mov rsi, 10
    call checkGreater
    
checkGreater:
    cmp rdi, rsi
    jg print1
    jle print0
print0:
    mov rax, 1        ; write(
    mov rdi, 1        ;   STDOUT_FILENO,
    mov rsi, zero
    mov rdx, 2
    syscall
    mov rax, 60       ; exit(
    mov rdi, 0        ;   EXIT_SUCCESS
    syscall
    ret
print1:
    mov rax, 1        ; write(
    mov rdi, 1        ;   STDOUT_FILENO,
    mov rsi, one
    mov rdx, 2
    syscall

    mov rax, 60       ; exit(
    mov rdi, 0        ;   EXIT_SUCCESS
    syscall
    ret

section .rodata
  one: db "1 ", 10
  zero: db "0 ", 10
