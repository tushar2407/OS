#include <stdio.h>
#include <inttypes.h>

int64_t long_add(int64_t, int64_t, int64_t, int64_t);
int64_t extended_long(int64_t, int64_t, int64_t, int64_t);

int main(){
    long a,b,c,d;
    scanf("%ld",&a);
    scanf("%ld",&b);
    scanf("%ld",&c);
    scanf("%ld",&d);
    long e =extended_long(a,b,c,d);
    printf("%ld",e);
    return 0;
}