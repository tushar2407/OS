section .data
	text db "-"
	global add_num
section .bss
    dig resb 100
    digPos resb 8

	section .text
add_num:
	add rdi, rsi
	mov rax, rdi
	mov r15, rax
	cmp rax, 0
	jl	_neg
	jge _pos
    
_pos:
	call _print
    mov rax, 60
    mov rdi, 0
    syscall

_neg:
	neg r15
	mov rax, 1
	mov rdi, 1
	mov rsi, text
	mov rdx, 1
	syscall
	mov rax, r15
	call _print
    mov rax, 60
    mov rdi, 0
    syscall

_print:
    mov rcx, dig
    mov rbx, 10
    mov [rcx], rbx
    inc rcx
    mov [digPos], rcx

_printLoop:
    mov rdx, 0
    mov r14, 10 ;
    mov rbx, 10
    mov r13, r14 ;
    div rbx
    push rax  
    add r13, r14  
    add rdx, 48
    
    mov rcx, [digPos]
    mov [rcx], dl
    inc rcx
    mov r14, r13
    mov [digPos], rcx

    pop rax
    cmp rax,0
    jne _printLoop
	
_printLoop2:
    mov rcx, [digPos]
    mov rax, 1
    mov rdi, 1
    mov rsi, rcx
    mov rdx, 1
    syscall

    mov rcx, [digPos]
    dec rcx
    mov r15, r14 ;
    mov [digPos], rcx

    cmp rcx, dig
    jge _printLoop2

    ret