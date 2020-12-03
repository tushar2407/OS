#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>
#include<sys/wait.h>

typedef struct
{
  pthread_mutex_t mutex1;
  char ch[100];
} shared_data1;

typedef struct
{
  pthread_mutex_t mutex2;
  long long int x;
} shared_data2;

typedef struct
{
  pthread_mutex_t mutex3;
} shared_data3;

shared_data1* data1 = NULL;//parent
shared_data2* data2 = NULL;//child
/* static */shared_data3* data3 = NULL; //writing

void initialise_shared()
{
   // place our shared data in shared memory
   int prot = PROT_READ | PROT_WRITE;
   int flags = MAP_SHARED | MAP_ANONYMOUS;
   data1 = mmap(NULL, sizeof(shared_data1), prot, flags, -1, 0);
   data2 = mmap(NULL, sizeof(shared_data2), prot, flags, -1, 0);
   data3 = mmap(NULL, sizeof(shared_data3), prot, flags, -1, 0);

   data2->x = 0;

   // initialise mutex so it works properly in shared memory
   pthread_mutexattr_t attr;
   pthread_mutexattr_init(&attr);
   pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
   pthread_mutex_init(&data1->mutex1, &attr);
   pthread_mutex_init(&data2->mutex2, &attr);
   pthread_mutex_init(&data3->mutex3, &attr);
}

void run_child()
{
   // child and writing is blocked
   printf("Locking for child to write Wait..\n");
   pthread_mutex_lock(&data1->mutex1); 
   pthread_mutex_lock(&data2->mutex2); 
   //pthread_mutex_lock(&data3->mutex3); 
   printf("Child locked, child writing\n");
   char temp_char = 'A';
   for(int i=99;i>=0;i--)
   {  
      data1->ch[i] = temp_char;
      if(temp_char == 'z')
      {
         temp_char = 'A';
      }
      temp_char++;
   }
   for(long long int i = 0;i<10000000;i++)
   {
      data2->x+=1;
   }
   printf("Writing complete, child wrote %lld\n", data2->x);
   printf("Unlocking locks in child after writing\n");
   pthread_mutex_unlock(&data1->mutex1); 
   pthread_mutex_unlock(&data2->mutex2); 
   //pthread_mutex_unlock(&data3->mutex3);
   printf("Locks unlocked in child after writing\n"); 

   //writing complete, now read
   printf("Locking for child to read\n");
   pthread_mutex_lock(&data1->mutex1);
   pthread_mutex_lock(&data2->mutex2);
   //pthread_mutex_lock(&data3->mutex3);
   printf("locked in child to read\n");
   printf("Text %s, Digits %lld\n", data1->ch, data2->x); 
   //pthread_mutex_unlock(&data3->mutex3);
   pthread_mutex_unlock(&data2->mutex2);
   pthread_mutex_unlock(&data1->mutex1);
   printf("Unlocking locks in child after reading\n");
   /*printf("Locking Child\n");
   pthread_mutex_lock(&data1->mutex1);
   pthread_mutex_lock(&data2->mutex2); 
   pthread_mutex_lock(&data3->mutex3); 
   printf("Inside Child\n");
   pthread_mutex_unlock(&data2->mutex2);
   pthread_mutex_unlock(&data1->mutex1);
   printf("Unlocked Child\n");*/
}

void run_parent(pid_t pid)
{
   // parent and writing is blocked
   printf("Blocking for parent to write Wait..\n");
   pthread_mutex_lock(&data1->mutex1); 
   pthread_mutex_lock(&data2->mutex2); 
   //pthread_mutex_lock(&data3->mutex3); //writing mutex locked
   printf("Parent Locked, Parent Writing\n");
   char temp_char = 'A';
   for(int i=0;i<99;i++)
   {  
      data1->ch[i] = temp_char;
      if(temp_char == 'z')
      {
         temp_char = 'A';
      }
      temp_char++;
   }
   for(long long int i = 0;i<10000000;i++)
   {
      data2->x+=1;
   }
   printf("Writing complete, parent wrote %s\n", data1->ch);
   printf("Writing complete, parent wrote %lld\n", data2->x);
   printf("Unlocking locks in parent\n");
   //pthread_mutex_unlock(&data3->mutex3);
   pthread_mutex_unlock(&data2->mutex2);
   pthread_mutex_unlock(&data1->mutex1);
   printf("Locks unlocked in parent after writing\n");

   //writing complete, now read
   printf("Locking for parent to read\n");
   pthread_mutex_lock(&data1->mutex1);
   pthread_mutex_lock(&data2->mutex2);
   //pthread_mutex_lock(&data3->mutex3);
   printf("locked in parent to read\n");
   printf("Text %s, Digits %lld\n", data1->ch, data2->x); 
   //pthread_mutex_unlock(&data3->mutex3);
   pthread_mutex_unlock(&data2->mutex2);
   pthread_mutex_unlock(&data1->mutex1);
   printf("Unlocked in parent after reading\n");
   /*printf("Locking Parent\n");
   pthread_mutex_lock(&data1->mutex1);
   pthread_mutex_lock(&data2->mutex2);
   pthread_mutex_lock(&data3->mutex3); 
   printf("Inside parent\n");
   pthread_mutex_unlock(&data2->mutex2);
   pthread_mutex_unlock(&data1->mutex1);
   printf("Unlocked Parent\n");*/
   wait(NULL);
}

int main()
{
   initialise_shared();
   pid_t pid = fork();
   if (pid>0) 
   {
      run_parent(pid);
   }
   else 
   {
      run_child();
   }

   munmap(data1, sizeof(data1));
   munmap(data2, sizeof(data2));
   munmap(data3, sizeof(data3));
   return 0;
}
