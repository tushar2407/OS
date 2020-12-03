/*
    References:
    https://www.geeksforgeeks.org/udp-server-client-implementation-c/
*/
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 

#define PORT	 8080 

// Driver code 
int main() { 
	int sockfd; 
	char *hello = "Hello from client 1"; 
	struct sockaddr_in	 serveraddress; 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&serveraddress, 0, sizeof(serveraddress)); 
	serveraddress.sin_family = AF_INET; 
	serveraddress.sin_port = htons(PORT); 
	serveraddress.sin_addr.s_addr = INADDR_ANY; 
	
	int n; 
	
	sendto(
        sockfd, 
        (const char *)hello, 
        strlen(hello), 
		MSG_CONFIRM, 
        (const struct sockaddr *) &serveraddress, 
		sizeof(serveraddress)
    ); 
	printf("Hello message sent from client 1\n"); 

    FILE *fptr;
	fptr = fopen("para1.txt", "r");
	char temp[100];
	while( fgets ( temp, 100, fptr ) != NULL ) 
    {    
		char* token = strtok(temp, " ");
		while (token != NULL) 
		{
			
			sendto(
                sockfd, 
                (const char *)token, 
                strlen(token), 
		        MSG_CONFIRM, 
                (const struct sockaddr *) &serveraddress, 
			    sizeof(serveraddress)
            ); 
	
			printf("Data sent from client 1 is : %s \n", token); 
        	token = strtok(NULL, " "); 
    	}
    }	
	close(sockfd); 
	return 0; 
} 