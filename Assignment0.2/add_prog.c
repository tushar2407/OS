#include <stdio.h>
#include <inttypes.h>

int64_t add_num(int64_t, int64_t);

int main(){
    long a,b;
    scanf("%ld",&a);
    scanf("%ld",&b);
    add_num(a,b);
    return 0;
}