/*
	References:
	https://www.geeksforgeeks.org/ipc-using-message-queues/
	
*/
#include <stdio.h> 
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
	msgid = msgget(key, 0666); 
	while(msgrcv(msgid, &message, sizeof(message), 1, IPC_NOWAIT)>0 ){
		printf("Data Received is : %s \n", 
					message.text); 
	}
	msgctl(msgid, IPC_RMID, NULL); 
	return 0; 
} 
