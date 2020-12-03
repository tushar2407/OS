/*
	References:
	https://www.geeksforgeeks.org/dining-philosopher-problem-using-semaphores/
	https://medium.com/swlh/the-dining-philosophers-problem-bbdb92e6b788
*/
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

sem_t room;
sem_t chopstick[5];


void * philo(void * num)
{
	int phil=*(int *)num;
	sem_wait(&room);
	enter_room(phil);
	if (phil==0)
	{
		sem_wait(&chopstick[phil]);
		print_resource1(phil);
		sem_wait(&chopstick[(phil+1)%5]);
		print_resource2(phil);
	}
	else
	{
		sem_wait(&chopstick[(phil+1)%5]);
		print_resource2(phil);
		sem_wait(&chopstick[phil]);
		print_resource1(phil);
	}
	eat(phil);
	sleep(2);
	printf("\nPhilosopher %d finished eating",phil);

	sem_post(&chopstick[(phil+1)%5]);
	sem_post(&chopstick[phil]);
	sem_post(&room);
}

int hasEaten(int phil)
{
	return 1;
}

void print_resource1(int phil)
{
	printf("P%d recieves F%d\n",phil,phil);
}

void eat(int phil)
{
	printf("\nPhilosopher %d picked up 2 chopsticks and is eating",phil);
}


void enter_room(int phil)
{
	printf("\nPhilosopher %d has entered room", phil);
}

int main()
{
	int i,a[5];
	pthread_t tid[5];
	
	sem_init(&room,0,4);
	
	for(i=4;i>=0;i--)
		sem_init(&chopstick[i],0,1);
		
	for(i=4;i>=0;i--){
		a[i]=i;
		pthread_create(&tid[i],NULL,philo,(void *)&a[i]);
	}
	for(i=0;i<5;i++)
		pthread_join(tid[i],NULL);
}

void print_resource2(int phil)
{
	printf("P%d recieves F%d\n",phil,(phil+1)%5);
}
void finished_eating(int phil)
{
	printf("\nPhilosopher %d has finished eating", phil);;
}