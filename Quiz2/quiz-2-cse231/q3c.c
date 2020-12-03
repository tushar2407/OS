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
#define MAXLINE  100

// Driver code 
int main() { 
	int sockfd; 
	char buffer[MAXLINE]; 
	char *message = "Hello from server"; 
	struct sockaddr_in serveraddress, clientaddress; 	
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) 
	{ 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	memset(&serveraddress, 0, sizeof(serveraddress)); 
	memset(&clientaddress, 0, sizeof(clientaddress)); 	
	serveraddress.sin_family = AF_INET; // IPv4 
	serveraddress.sin_addr.s_addr = INADDR_ANY; 
	serveraddress.sin_port = htons(PORT); 
	
	if ( bind(sockfd, (const struct sockaddr *)&serveraddress, 
			sizeof(serveraddress)) < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	int len, n; 
	len = sizeof(clientaddress); //len is value/resuslt 
	
	while((n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
				MSG_WAITALL, ( struct sockaddr *) &clientaddress, 
				&len))>0)
	{
		buffer[n] = '\0'; 
		printf("Client : %s\n", buffer); 
	}
	
	return 0; 
} 
