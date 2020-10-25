default:
	nasm -felf64 P1.asm -o P1.o
	gcc -c P1.c -o P1_1.o
	ld -e main P1.o P1_1.o -o P1
	./P1