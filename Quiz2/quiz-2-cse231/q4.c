/*
	references:
	https://www.geeksforgeeks.org/mutex-vs-semaphore/
	https://www.geeksforgeeks.org/semaphores-in-process-synchronization/
	https://www.geeksforgeeks.org/use-posix-semaphores-c/
*/
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>	
#include <sys/mman.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/wait.h>

typedef struct
{
  pthread_mutex_t mutex;
  char ch[100];
} shared_data1;

int *a, *b;
char *c, *d;

int main()
{

	shared_data1* data1 = NULL;//parent
	data1 = mmap(
		NULL, 
		sizeof(shared_data1),
		PROT_READ | PROT_WRITE, 
		MAP_SHARED | MAP_ANONYMOUS, 
		-1, 
		0
		);
	pthread_mutexattr_t attr;
   	pthread_mutexattr_init(&attr);
   	pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
   	pthread_mutex_init(&data1->mutex, &attr);
	
	int shmid1, shmid2, status;
	int i;
	shmid1 = shmget(IPC_PRIVATE, 2*sizeof(int), 0777|IPC_CREAT);
	shmid2 = shmget(IPC_PRIVATE, 1024, 0777|IPC_CREAT);

	pid_t pid = fork();
	if (pid == 0) {
		b = (int *) shmat(shmid1, 0, 0);
		d = (char *) shmat(shmid2, 0, 0);

		for( i=0; i< 10; i++) {
			// sleep(1);
			pthread_mutex_lock(&data1->mutex);
			printf("\t\t\t Child reads: %d,\t %d\n",b[0],b[1]);
			printf("%s\n",d);
			pthread_mutex_unlock(&data1->mutex);
		}
		b[0] = 0; b[1] = 1;
		strcpy(d,"child");
		for( i=0; i< 10; i++) {
			// sleep(1);
			pthread_mutex_lock(&data1->mutex); 
			b[0] = b[0] + b[1];
			b[1] = b[0] + b[1];
			strcat(d,"child");
			pthread_mutex_unlock(&data1->mutex); 
			printf("child writes: %d,%d\n",b[0],b[1]);
		}
		shmdt(b);
		shmdt(d);
	}
	else {
		// parent 
		a = (int *) shmat(shmid1, 0, 0);
		c = (char *) shmat(shmid2, 0, 0);
		
		a[0] = 0; a[1] = 1;
		strcpy(c,"parent");
		for( i=0; i< 10; i++) {
			// sleep(1);
			// sem_wait(&mutex);
			pthread_mutex_lock(&data1->mutex); 
			a[0] = a[0] + a[1];
			a[1] = a[0] + a[1];
			strcat(c,"parent");
			// sem_post(&mutex);
			pthread_mutex_unlock(&data1->mutex); 
			printf("Parent writes: %d,%d\n",a[0],a[1]);
		}

		for( i=0; i< 10; i++) {
			// sleep(1);
			// sem_wait(&mutex);
			pthread_mutex_lock(&data1->mutex);
			printf("\t\t\t Parent reads: %d,\t %d\n",a[0],a[1]);

			printf("%s\n",c);
			pthread_mutex_unlock(&data1->mutex);
			// sem_post(&mutex);
		}
		

		wait(NULL);
		shmdt(a);
		shmdt(c);
		shmctl(shmid1, IPC_RMID, 0);
		shmctl(shmid2, IPC_RMID, 0);
	}
	return 0;
}