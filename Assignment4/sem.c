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
  while(pthread_mutex_trylock(&s->m)){
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
  while(pthread_mutex_trylock(&s->m)){
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
sem_t chopstick[5];
sem_t bowl[2];
void * philo(void * num) // blocking
{
	// while(1){
    int phil=*(int *)num;
	sem_wait_nonblock(&room);
	sem_wait_nonblock(&chopstick[phil]);
	sem_wait_nonblock(&chopstick[(phil+1)%5]);
    sem_wait_nonblock(&bowl[0]);
    sem_wait_nonblock(&bowl[1]);
	sleep(2);
    printf("Philosopher %d eats using forks %d and %d.\n", phil, phil, (phil+1)%5);
    // printf("Philosopher %d got forks %d and %d\n", phil, phil, (phil+1)%5);
	// printf("Philosopher %d finished eating\n",phil);
	sem_post_nonblock(&chopstick[(phil+1)%5]);
	sem_post_nonblock(&chopstick[phil]);
    sem_post_nonblock(&bowl[0]);
    sem_post_nonblock(&bowl[1]);
	sem_post_nonblock(&room);
    // }
    
}
int main()
{
	int i,a[5];
	
	sem_init(&room,4);
	
	while(1){
        pthread_t tid[5];
        for(i=4;i>=0;i--)
		    sem_init(&chopstick[i],1);
		sem_init(&bowl[0],1);
		sem_init(&bowl[1],1);
	    for(i=0;i<5;i++){
	    	a[i]=i;
	    	pthread_create(&tid[i],NULL,philo,(void *)&a[i]);
	    }
	    for(i=0;i<5;i++)
	    	pthread_join(tid[i],NULL);
    }
}