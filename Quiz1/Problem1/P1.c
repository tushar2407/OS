#include<stdio.h>
extern int checkGreater(int, int);
int main(){
    int x=50,y=10;
    checkGreater(x, y);
}


/*
nasm -felf64 problem1.asm -o P1.o
gcc -c problem1.c -o P1_1.o
ld -e main P1.o P1_1.o -o P1
./P1
*/