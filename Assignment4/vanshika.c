#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

#define N 5   //number of philosophers and forks
#define THINKING 2 //variable for thinking state
#define HUNGRY 1  //variable for hungry state
#define EATING 0  //variable for eating state
#define LEFT (phnum + (N - 1)) % N  //calculating left philosopher number
#define RIGHT (phnum + 1) % N  //calculating right philosopher number

int state[N];  //array for states
int philos[N] = { 0, 1, 2, 3, 4 };  //philospher number array , to pass to the threads 

//both availabe in pthread library
typedef pthread_mutex_t Cmutex; //mutex to be used in semaphore structure
typedef pthread_cond_t Conds; //conditional variable to be used in semaphore structure

typedef struct my_semaphore my_semaphore;

//strcuture of semaphore my_semaphore
struct my_semaphore{
  int value, wakeups;  //value of the semaphore and wakeup to record the number of times the semaphore is signaled 
  Cmutex *mutex;  //mutex for synchronization
  Conds *cond;  //condition variable for synchronization
};

void perror_exit(char *s) { //error msg dislayer
  perror(s);  
  exit(-1);
}

//function to allocate any variable
void *check_allocation(int size) {
  void *p = malloc(size);
  if (p == NULL){
   perror_exit("malloc failed");
  }
  return p;
}

//function to initialize the mutex in the semaphore structure
Cmutex *init_mutex() {
  Cmutex *mutex = check_allocation(sizeof(Cmutex));
  int n = pthread_mutex_init(mutex, NULL);
  if (n != 0){
   perror_exit("make_lock failed"); 
  }
  return mutex;
}

//function to initialize the condition variable in the semaphore structure
Conds *init_cond() {
  Conds *cond = check_allocation(sizeof(Conds)); 
  int n = pthread_cond_init(cond, NULL);
  if (n != 0){ 
   perror_exit("make_cond failed");
  }
  return cond;
}

//funtion to initialize the semaphore
my_semaphore *initiate_semaphore(int value) {
//Assigning Values
  my_semaphore *semaphore = check_allocation(sizeof(my_semaphore));
  semaphore->value = value;
  semaphore->wakeups = 0;
  semaphore->mutex = init_mutex();
  semaphore->cond = init_cond();
  return semaphore;
}

//function to wait/down the semaphore
void semaphore_wait(struct my_semaphore *semaphore) {
  pthread_mutex_lock(semaphore->mutex); //locking the mutex
  semaphore->value--;  //decreasing value of the semaphore
  if (semaphore->value < 0) {
    do {
      pthread_cond_wait(semaphore->cond, semaphore->mutex); //waiting/downing the condition variable
    } while (semaphore->wakeups < 1);
    semaphore->wakeups--; //dcrementing the number of times the semaphore is used
  }
  pthread_mutex_unlock(semaphore->mutex); //unlocking the mutex
}

//function to signal/up the semaphore
void semaphore_signal(struct my_semaphore *semaphore) {
   pthread_mutex_lock(semaphore->mutex); //locking the mutex
  semaphore->value++; //increasing value of the semaphore
  if (semaphore->value <= 0) { 
    semaphore->wakeups++; //incrementing the number of times the semaphore is used
    pthread_cond_signal(semaphore->cond); //signaling the condition variable
  }
   pthread_mutex_unlock(semaphore->mutex);  //unlocking the mutex
}  

//function to get value of the semaphore in variable val
void semaphore_signal_value(struct my_semaphore *semaphore, int * val){
  *val = semaphore->value;
}
  
my_semaphore * semaphore;  //counting semaphore 
my_semaphore * forks[N]; //resource variable for forks
my_semaphore * saucebowls[2];  //resource varible for sauce bowls
  
//function to check if the philosophers on the left and right are eating or not, if not then the philosopher can eat
void check_left_right(int phnum){ 
    if (state[phnum] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING){ 
        // state that eating 
        state[phnum] = EATING; 
        semaphore_wait(saucebowls[0]); //waiting for sauce bwol 1 to get free
        semaphore_wait(saucebowls[1]); //waiting for sauce bwol 2 to get free
        sleep(2); //wait time to make output readable on console
        printf("Philosopher %d eats using forks %d and %d\n", phnum + 1, LEFT + 1, phnum + 1); 
        semaphore_signal(forks[phnum]); 
        semaphore_signal(saucebowls[0]); //signal that sauce bowl 1 is free
        semaphore_signal(saucebowls[1]); //signal that sauce bowl 2 is free
    } 
}
  
// function to get the forks on left and right by checking the condition using above function
void get_forks(int phnum){ 
    semaphore_wait(semaphore); //waiting till the last philospher has returned the forks
    state[phnum] = HUNGRY;   //philosopher in hungry state before eating
    check_left_right(phnum); //check if philosopher has freed the fork 
    semaphore_signal(semaphore); //signaling semaphore so that philosopher can return the forks
    semaphore_wait(forks[phnum]); //waiting for the fork to get free by philosopher
} 
  
//function to return the forks after eating so that left and right philosophers can eat
void return_forks(int phnum){ 
    semaphore_wait(semaphore); //waiting for philosopher to finish eating 
    state[phnum] = THINKING; //philosopher goes to Thinking state after eating
    check_left_right(LEFT); //signaling the left philosopher using function that he can take fork
    check_left_right(RIGHT);  //signaling the right philosopher using function that he can take fork
    semaphore_signal(semaphore); //signaling so that next philosopher can eat
} 

//function to run the philosophers, first they get forks and then they return it.  
void* philospher(void* num){ 
    while(1) { 
        int* i = num; 
        get_forks(*i); 
        return_forks(*i); 
    } 
} 
  
int main() { 
    int i; 
    pthread_t thread_ids[N];  //array of thread ids
    // initialize the semaphores 
    semaphore = initiate_semaphore(N); //counting semaphore
    
    //initializing the resource variable for forks
    for (i = 0; i < N; i++){ 
        forks[i] = initiate_semaphore(0);
    }
    //initailizing the resource variable for sauce bowls
    for (i = 0; i < 2; i++) {
        saucebowls[i] = initiate_semaphore(1); 
    }
    //creating thred for each philosopher
    for (i = 0; i < N; i++) { 
        pthread_create(&thread_ids[i], NULL, philospher, &philos[i]); 
    } 
    //joining threads of all the philosophers
    for (i = 0; i < N; i++){  
        pthread_join(thread_ids[i], NULL);
    } 
}