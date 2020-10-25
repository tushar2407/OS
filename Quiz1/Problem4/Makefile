default:
	nasm -felf32 P4_1.asm -o P4.o
	gcc -m32 P4.c P4.o -o P4
	./P4

bit64:
	nasm -felf64 P4_2.asm -o P4.o
	gcc P4.c P4.o -o P4
	./P4