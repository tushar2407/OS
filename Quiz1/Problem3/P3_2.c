#include <stdio.h>
int main(){
    char x[64];
    int *a=(int*)&x;
    int *b=(int*)&(x[64]);
    int val=500;
    for(; a!=b; a++){
        *a=val;
        printf("%d\n",*a);
        val+=100;
    }
    
}