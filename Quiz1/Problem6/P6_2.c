#include<stdio.h>
#include<stdlib.h>
int main(){
    char s[30];
    // scanf("%s",s);
    FILE *f;
    f = fopen("A6_2.txt","w");
    printf("Input a string to be saved in the disk: ");
    fgets(s, sizeof(s), stdin );
    fprintf(f, "%s", s);
    fclose(f);
    // printf("input : ");
    // puts(s);
}