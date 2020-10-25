#include <stdio.h>
int main(){
    char x[64];
    long int *a=(long int*)&x;
    long int *b=(long int*)&(x[64]);
    long int val=1000;
    for(; a!=b;a++){
        *a=val;
        printf("%ld\n",*a);
        val+=1000;
    }
}