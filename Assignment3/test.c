/* Name: Tushar Mohan
   Roll_Number: 2019393 */

#include <sys/time.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <time.h>
#include <unistd.h>

int main()
{
	
	long pid=fork();
	// scanf("%ld", &pid);
	// syscall(440,pid,100000000);
	// return 0;
	long i=0;
	if (pid<0)
		printf("Fork failed");
	
	else if(pid==0){
		int temp = getpid();
        printf("Child %d", temp);
		syscall(440,temp,100000000);
		clock_t t;
		double time_taken;
		t = clock();
		for (i=0; i<5000000000; i++);
		t = clock() - t;
		time_taken = ((double)t)/CLOCKS_PER_SEC;
		printf("Child took %f seconds to execute \n", time_taken);
	}
	else{
		int temp = getpid();
		printf("Parent %d", temp);
		clock_t t;
		double time_taken;
		t = clock();
		for (i=0; i<5000000000; i++);
		t = clock() - t;
		time_taken = ((double)t)/CLOCKS_PER_SEC;
		printf("Parent took %f seconds to execute \n", time_taken);
	    wait(NULL);
	}
	
	
	return 0;

}