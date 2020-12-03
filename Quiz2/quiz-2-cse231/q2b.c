/*
	References:
	https://www.geeksforgeeks.org/ipc-using-message-queues/
	
*/
#include <stdio.h> 
#include <string.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 

struct mesg_struct { 
	long type; 
	char text[100]; 
} message; 

int main() 
{ 
	key_t key; 
	int msgid; 
	key = ftok("tushar", 65); 
	msgid = msgget(key, 0666 | IPC_CREAT); 
	message.type = 1; 

	FILE *fptr;
	fptr = fopen("para2.txt", "r");
	char temp[100];
	while( fgets ( temp, 100, fptr ) != NULL ) 
    {    
		char* token = strtok(temp, " ");
		while (token != NULL) 
		{ 
			strcpy(message.text, token);
			msgsnd(msgid, &message, sizeof(message), 0); 
			printf("Data send is : %s \n", message.text); 
        	token = strtok(NULL, " "); 
    	}
    } 
    printf("The end");
	return 0; 
} 
