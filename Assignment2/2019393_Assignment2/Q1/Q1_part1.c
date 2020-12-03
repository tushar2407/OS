/* Name: Tushar Mohan
   Roll_Number: 2019393 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
int x = 10;
int main(){
   pid_t pid = fork();
   if(pid<0){
      printf("Unsuccessful\n");
   }
   else if (pid==0){
      // child process
      printf("%d Child process\n", x);
      while(x!=-90){
         printf("%d\n",--x);
      }
   }
   else{
      //parent process
      printf("%d Parent process\n", x);
      while(x!=100){
         printf("%d\n",++x);
      }
      wait(NULL);
   }
   return 0;
}