global _start
global print1
global print0
section .text
_start: 
    mov rdi, 5
    cmp rdi, 10
    jg print0
    jle print1
    
    
print0:
    mov rax, 1        ; write(
    mov rdi, 1        ;   STDOUT_FILENO,
    mov rsi, zero
    mov rdx, 1
    syscall
    mov rax, 60       ; exit(
    mov rdi, 0        ;   EXIT_SUCCESS
    syscall
    ret
print1:
    mov rax, 1        ; write(
    mov rdi, 1        ;   STDOUT_FILENO,
    mov rsi, one
    mov rdx, 1
    syscall

    mov rax, 60       ; exit(
    mov rdi, 0        ;   EXIT_SUCCESS
    syscall
    ret

section .rodata
  one: db "1", 10
  zero: db "0", 10
