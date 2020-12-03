#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<ctype.h>
#include<unistd.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define PORT 9000 //port number of server 
#define MAX 300

int main(int argc, char* argv[]){
    char data[100]; //buffer for rec message from server 
    char start_mesg[100]="FLAG_AKG_CHECK";
    char mesg[100];
    int n;
    int sockfd;
    struct sockaddr_in servaddr ;

    //providng server add to socket interface 
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    servaddr.sin_port=htons(PORT);
    servaddr.sin_family=AF_INET;

    //making udp socket 
    sockfd=socket(AF_INET, SOCK_DGRAM, 0);
    //connecting to server now 
    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))<0){
        printf("\n Error:Connect Failed\n");
        exit(0);
    }

    //request for datagram fro server 
    sendto(sockfd, "FLAG_AKG_CHECK", MAX, 0, (struct sockaddr*)NULL, sizeof(servaddr));

    //running infinite loop to receive data from server 
    while(1){
        recvfrom(sockfd, data, sizeof(data), 0, (struct sockaddr*)NULL, NULL);

        
          printf("received : %s", data);
        
    }
    close(sockfd);
}
