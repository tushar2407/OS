#include<stdio.h>
#include<stdlib.h>
int main(){
    char *s=malloc(20);
    printf("Input a string: ");
    scanf("%s",s);
    printf("input : ");
    puts(s);
}