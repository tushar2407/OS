#include<stdio.h>
#include <unistd.h>
ssize_t write(int fd, const void *buf, size_t count);
__int64_t start1();
int main(){
    write(1,"Hello",5);
    start1();
}