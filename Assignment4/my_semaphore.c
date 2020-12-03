#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define P 5
#define LEFT (phil + (P - 1)) % P  //calculating left philosopher number
#define RIGHT (phil + 1) % P  //calculating right philosopher number
#define THINKING 2
#define HUNGRY 1
#define EATING 0
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
static const int N = 5;
static const int Z = 200000;
static int COUNTER = 0;
Sam forks[5];
Sam saucebowls[2];
Sam semaphore; //counting semaphore
int state[5];
void tell_left_right(int phil)
{
    if(state[phil]==HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        state[phil] = EATING; // eating
        sam_acquire(&saucebowls[0]); //wait for bowl 1
        sam_acquire(&saucebowls[1]);
	    sleep(2);
        printf("Philsopher %d recieves Fork %d and Fork%d\n",phil+1,LEFT+1,phil+1);    
	    sam_release(&forks[phil]);
	    // sam_release(&forks[(phil+1)%5]);
        sam_release(&saucebowls[0]);
        sam_release(&saucebowls[1]);
    }
}
void * philo(void * num)
{
	while(1){
        int phil=*(int *)num;
	    // get forks
        // printf("Getting forks\n");
        sam_acquire(&semaphore);
        state[phil] = HUNGRY; // thinking as not eating and not hungry
        // check left and right
        if(state[phil]==HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
        {
            state[phil] = EATING; // eating
            sam_acquire(&saucebowls[0]); //wait for bowl 1
            sam_acquire(&saucebowls[1]);
	        sleep(2);
            printf("Philsopher %d recieves Fork %d and Fork%d\n",phil+1,LEFT+1,phil+1);    
	        sam_release(&forks[phil]);
	        // sam_release(&forks[(phil+1)%5]);
            sam_release(&saucebowls[0]);
            sam_release(&saucebowls[1]);
        }
        sam_release(&semaphore);
        sam_acquire(&forks[phil]);
        // now return the forks
        // printf("Returning forks\n");
        sam_acquire(&semaphore);
        state[phil] = THINKING;
        // signal left and right that done
        tell_left_right(LEFT);
        tell_left_right(RIGHT);
        sam_release(&semaphore);
        // printf("Philosopher %d finished eating\n",phil);
    }
	// sam_release(&room);
}

int main(void) {
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
    // fprintf(stderr,"%d\n", COUNTER);
    return EXIT_SUCCESS;
}