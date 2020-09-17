all: preprocess compile object link execute
preprocess: add_prog.c
	gcc -E add_prog.c -o add_prog.i
compile: add_prog.c
	gcc -S add_prog.c
object: add_prog.c add_asm.asm
	nasm -felf64 add_asm.asm -o add_asm.o
	gcc -c add_prog.c -o add_prog.o
link: add_prog.c add_asm.o
	gcc add_prog.c add_asm.o -static -o add
execute: add
	./add