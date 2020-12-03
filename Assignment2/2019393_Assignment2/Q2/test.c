/* Name: Tushar Mohan
   Roll_Number: 2019393 */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
int main(){
   long pid;
   printf("Enter the pid :");
   scanf(" %ld", &pid);
   char file[1024], temp[1024];
   printf("Enter the filename:");
   scanf(" %s", file);
   strcpy(temp, "./");
   strcat(temp, file);
   int file_path_len = strlen(temp);
   int x = syscall(440, pid, (const char *)temp, file_path_len);
   printf("%s\n", strerror(x));
   return 0;
}  