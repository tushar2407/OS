#include <stdio.h>
#include <inttypes.h>

int32_t long_add(int32_t, int32_t, int32_t, int32_t);
// int64_t extended_long(int64_t, int64_t, int64_t, int64_t);

int main(){
    int a,b,c,d;
    scanf("%d",&a);
    scanf("%d",&b);
    scanf("%d",&c);
    scanf("%d",&d);
    int e =long_add(a,b,c,d);
    printf("%d",e);
    return 0;
}