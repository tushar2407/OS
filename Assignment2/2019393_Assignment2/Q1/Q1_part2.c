/* Name: Tushar Mohan
   Roll_Number: 2019393 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
int x=10;
void *add(void* arg){
   pthread_detach(pthread_self()); 
   printf("%d Child\n", x);
   while(x!=-90){
      printf("Child %d\n",--x)   ;
   }
   pthread_exit(NULL);
}
int main(){
   pthread_t ptid;
   pthread_create(&ptid, NULL, &add, NULL);
   printf("%d Parent\n", x);
   while(x!=100){
      printf("Parent %d\n",++x);
   }
   pthread_join(ptid, NULL);
   pthread_exit(NULL);
   return 0;
}