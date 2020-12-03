/*
	References:
	https://www.geeksforgeeks.org/mutex-lock-for-linux-thread-synchronization/?ref=rp
	
*/
#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 

pthread_t tid[10]; 
int ctr; 
pthread_mutex_t lock; 
unsigned long i = 0; 
char *str;
int shmid;

void *read_me(void* arg);
void *write_me(void* arg);

void join_threads(){
	pthread_join(tid[0], NULL); 
	pthread_join(tid[1], NULL); 
	pthread_join(tid[2], NULL); 
	pthread_join(tid[3], NULL); 
	pthread_join(tid[4], NULL); 
	pthread_join(tid[5], NULL); 
	pthread_join(tid[6], NULL); 
	pthread_join(tid[7], NULL); 
	pthread_join(tid[8], NULL); 
	pthread_join(tid[9], NULL); 
	
}
int main(void) 
{ 
	int i = 0; 
	int error; 
	int a[10] = {1,2,3,4,5,6,7,8,9,10};
	if (pthread_mutex_init(&lock, NULL) != 0) { 
		printf("\n mutex init failed\n"); 
		return 1; 
	} 
	key_t key = ftok("tushar",65); 
    int shmid = shmget(key,1024,0666|IPC_CREAT); 
    
	str = (char*) shmat(shmid,(void*)0,0); 
	
	error = pthread_create(
		&(tid[i]), 
		NULL, 
		&write_me, 
		(void *)&a[i]
	); 
	if (error != 0) 
	{
		printf(
			"\nThread can't be created :[%s]", 
			strerror(error)
		); 
	}
	i++; 

	while (i < 10) { 
		error = pthread_create(
			&(tid[i]), 
			NULL, 
			&read_me, 
			(void *)&a[i]
		); 
		if (error != 0)
		{ 
			printf(
				"\nThread can't be created :[%s]", 
				strerror(error)
			); 
		}
		i++; 
	} 

	join_threads();
	
	pthread_mutex_destroy(&lock); 
	
	shmdt(str); 
    
    // destroy the shared memory 
    shmctl(shmid,IPC_RMID,NULL); 

	return 0; 

} 
void* read_me(void* arg)
{
	ctr ++; 
	printf("read as %s by Reader %d\n", str, *((int *)arg));
}

void* write_me(void* arg) 
{ 
	
	ctr++;
	printf("\n Reader %d has started\n", *((int *)arg)); 

	char ch = 'A';
	for ( int i=0; i<1000; i++ )
	{
		pthread_mutex_lock(&lock); 
		str[i%8]=ch;
		if(ch=='Z')
			ch='A';
		else
			ch++;
		pthread_mutex_unlock(&lock); 
	}
	
	printf("\n Job %d has finished\n", *((int *)arg)); 

	return NULL; 
}