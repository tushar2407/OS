/*
	References:
	  https://www.geeksforgeeks.org/dining-philosopher-problem-using-semaphores/
	  https://medium.com/swlh/the-dining-philosophers-problem-bbdb92e6b788
    http://cs241.cs.illinois.edu/coursebook/Synchronization#implementing-counting-semaphore
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

typedef struct sem_t {
  ssize_t count;
  pthread_mutex_t m;
  pthread_cond_t cv;
} sem_t;

void debug_sem_value(struct sem_t *s)
{
    printf("Value of semaphore is %ld\n", s->count);
}

int sem_init(sem_t *s, int value) {
  s->count = value;
  pthread_mutex_init(&s->m, NULL);
  pthread_cond_init(&s->cv, NULL);
  return 0;
}

void sem_post(sem_t *s) {
  pthread_mutex_lock(&s->m);
  s->count++;
  pthread_cond_signal(&s->cv);
  pthread_mutex_unlock(&s->m);
}

void sem_post_nonblock(sem_t *s) {
  while(pthread_mutex_trylock(&s->m) && s->count>0){
    s->count--;
    pthread_mutex_unlock(&s->m);
  }
  s->count++;
  pthread_cond_signal(&s->cv);
  pthread_mutex_unlock(&s->m);
}

void sem_wait(sem_t *s) {
  pthread_mutex_lock(&s->m);
  while (s->count == 0) {
    pthread_cond_wait(&s->cv, &s->m); /*unlock mutex, wait, relock mutex*/
  }
  s->count--;
  pthread_mutex_unlock(&s->m);
}

void sem_wait_nonblock(sem_t *s) {
  while(pthread_mutex_trylock(&s->m) && s->count>0){
    s->count--;
    pthread_mutex_unlock(&s->m);
  }
  while (s->count == 0) {
    pthread_cond_wait(&s->cv, &s->m); /*unlock mutex, wait, relock mutex*/
  }
  s->count--;
  pthread_mutex_unlock(&s->m);
}

sem_t room;
sem_t *chopstick;
sem_t bowl[2];
int n = 5;

void * philo(void * num) // blocking
{
    int phil=*(int *)num;
    sem_wait(&bowl[0]);
    sem_wait(&bowl[1]);
    if(phil%2==0){
      sem_wait(&chopstick[phil]);
      sem_wait(&chopstick[(phil+1)%n]);
	    sleep(2);
      printf("Philosopher %d eats using forks %d and %d.\n", phil+1, phil+1, ((phil+1)%n)+1);
	    sem_post(&chopstick[phil]);
      sem_post(&chopstick[(phil+1)%n]);
    }
    else{
      sem_wait(&chopstick[(phil+1)%n]);
      sem_wait(&chopstick[phil]);
      sleep(2);
      printf("Philosopher %d eats using forks %d and %d.\n", phil+1, phil+1, ((phil+1)%n)+1);
      sem_post(&chopstick[(phil+1)%n]);
      sem_post(&chopstick[phil]);
    }
	  
    sem_post(&bowl[0]);
    sem_post(&bowl[1]);
    
}

void * philo_non_blocking(void * num) // non-blocking
{
    int phil=*(int *)num;
    sem_wait_nonblock(&bowl[0]);
    sem_wait_nonblock(&bowl[1]);
    if(phil%2==0){
      sem_wait_nonblock(&chopstick[phil]);
      sem_wait_nonblock(&chopstick[(phil+1)%n]);
	    sleep(2);
      printf("Philosopher %d eats using forks %d and %d.\n", phil+1, phil+1, ((phil+1)%n)+1);
	    sem_post_nonblock(&chopstick[phil]);
      sem_post_nonblock(&chopstick[(phil+1)%n]);
    }
    else{
      sem_wait_nonblock(&chopstick[(phil+1)%n]);
      sem_wait_nonblock(&chopstick[phil]);
      sleep(2);
      printf("Philosopher %d eats using forks %d and %d.\n", phil+1, phil+1, ((phil+1)%n)+1);
      sem_post_nonblock(&chopstick[(phil+1)%n]);
      sem_post_nonblock(&chopstick[phil]);
    }
    sem_post_nonblock(&bowl[0]);
    sem_post_nonblock(&bowl[1]);
}

void blocking()
{
  int i,a[n];
  while(1){
      pthread_t tid[n];
      for(i=n-1;i>=0;i--)
		  sem_init(&chopstick[i],1);
		  sem_init(&bowl[0],1);
		  sem_init(&bowl[1],1);
	    for(i=0;i<n;i++){
	    	a[i]=i;
	    	pthread_create(&tid[i],NULL,philo,(void *)&a[i]);
	    }
	    for(i=0;i<n;i++)
	    	pthread_join(tid[i],NULL);
    }
}

void non_blocking()
{
  int i,a[n];
  while(1){
      pthread_t tid[n];
      for(i=n-1;i>=0;i--)
		  sem_init(&chopstick[i],1);
		  sem_init(&bowl[0],1);
		  sem_init(&bowl[1],1);
	    for(i=0;i<n;i++){
	    	a[i]=i;
	    	pthread_create(&tid[i],NULL,philo_non_blocking,(void *)&a[i]);
	    }
	    for(i=0;i<n;i++)
	    	pthread_join(tid[i],NULL);
  }
}
int main()
{
  printf("How many philsophers : ");
  scanf("%d", &n);
	printf("Blocking ?\n0=Yes \t 1=No\n");
  
  int choice;
  scanf("%d", &choice);
  
  int i,a[n];
  chopstick = malloc(n*sizeof(struct sem_t));
	sem_init(&room,1);
	switch (choice)
  {
    case 0:blocking();
      break;
    case 1:non_blocking();
      break;
    default:
      printf("Wrong choice input\n Terminating...\n");
      break;
  }
}