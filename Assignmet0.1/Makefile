output: object
	gcc hello.c
object: compile
	gcc hello.c -o hello.o
compile: preprocess
	gcc -S hello.c 
preprocess: 
	gcc -E hello.c > hello.txt
all: ouput