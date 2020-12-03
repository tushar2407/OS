#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
typedef struct {
    pthread_cond_t cond;
    pthread_mutex_t mtx;
    volatile unsigned N;
    volatile int wakeup;
} Sam;
#define SAM_INIT(n)                    \
{                                      \
    .cond = PTHREAD_COND_INITIALIZER,  \
    .mtx  = PTHREAD_MUTEX_INITIALIZER, \
    .N    = n                          \
};
void sam_init(Sam * ps, int N) {
    ps->N = N;
    ps-> wakeup = 0;
    pthread_mutex_init(&ps->mtx,NULL);
    pthread_cond_init(&ps->cond,NULL);
}
void sam_destroy(Sam * ps) {
    pthread_cond_destroy(&ps->cond);
    pthread_mutex_destroy(&ps->mtx);
}
void sam_acquire(Sam * ps) {
    pthread_mutex_lock(&ps->mtx);
    ps->N--;  //decreasing value of the semaphore
    if (ps->N < 0) {
      do {
        pthread_cond_wait(&ps->cond, &ps->mtx); //waiting/downing the condition variable
      } while (ps->wakeup < 1);
      ps->wakeup--; //dcrementing the number of times the semaphore is used
    }
  
    // ps->N--;
    // while(ps->N == 0) {
    //     pthread_cond_wait(&ps->cond,&ps->mtx);
    // }
    // --(ps->N);
    pthread_mutex_unlock(&ps->mtx);
}
void sam_release(Sam * ps) {
    pthread_mutex_lock(&ps->mtx);
    ps->N++; //increasing value of the semaphore
    if (ps->N <= 0) { 
      ps->wakeup++; //incrementing the number of times the semaphore is used
      pthread_cond_signal(&ps->cond); //signaling the condition variable
    }
  
    // ++(ps->N);
    // pthread_cond_signal(&ps->cond);
    pthread_mutex_unlock(&ps->mtx);
}
static const int N = 20;
static const int Z = 200000;
static int COUNTER = 0;
Sam forks[5];
Sam saucebowls[2];
Sam room;
void *thread_func(void *semaphore) {
    Sam * mumm = semaphore;

    sam_acquire(mumm);
    for(int i = 0; i < Z; i++) {
        ++COUNTER;
    }
    sam_release(mumm);
    return NULL;
}
void * philo(void * num)
{
	int phil=*(int *)num;
	// sam_acquire(&room);
	// enter_room(phil);
    // printf("\nPhilosopher %d has entered room", phil);
	if (phil==0)
	{
		sam_acquire(&forks[phil]);
		// print_resource1(phil);
        printf("Philsopher %d recieves F%d\n",phil,phil);
		sam_acquire(&forks[(phil+1)%5]);
		// print_resource2(phil);
        printf("Philosopher %d recieves F%d\n",phil,(phil+1)%5);
	}
	else
	{
		sam_acquire(&forks[(phil+1)%5]);
		// print_resource2(phil);
        printf("Philosopher %d recieves F%d\n",phil,(phil+1)%5);
		sam_acquire(&forks[phil]);
		// print_resource1(phil);
        printf("Philosopher %d recieves F%d\n",phil,phil);
	}
	// eat(phil);
    printf("Philosopher %d picked up 2 chopsticks and is eating\n",phil);
	sleep(2);
	printf("Philosopher %d finished eating\n",phil);

	sam_release(&forks[(phil+1)%5]);
	sam_release(&forks[phil]);
	// sam_release(&room);
}
Sam semaphore;
int main(void) {
    // Sam Mumm = SAM_INIT(1);
    // Sam Mumm;
    // sam_init(&Mumm, 1);
    int a[5];
    pthread_t threads[5];
    sam_init(&semaphore, 5);
    sam_init(&saucebowls[0], 1);
    sam_init(&saucebowls[2], 1);
    // sam_init(&saucebowls[2], 1);
    for(int i = 0; i < 5; i++)
        sam_init(&forks[i], 0);
    for(int i = 0; i < 5; i++) {
        a[i] = i;
        pthread_create(&threads[i],NULL,philo,(void *)&a[i]);
    }
    for(int i = 0; i < N; i++) {
        pthread_join(threads[i],NULL);
    }
    // sam_destroy(&Mumm);
    fprintf(stderr,"%d\n", COUNTER);
    return EXIT_SUCCESS;
}