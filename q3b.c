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
#define len1 100
#define one 1
#define len2 200
#define two 2
#define PORT 9000 //port no of server
#define MAX 300

int main(int argc, char* argv[]){
    int sockfd;
    //making buffer array for data 
    char data[100];
    char message[100]="FLAG_AKG_CHECK";
    int listenfd;
    int len;
    struct sockaddr_in servaddr, clieaddr;
    bzero(&servaddr, sizeof(servaddr));

    //making socket datagram ipaddress
    listenfd=socket(AF_INET, SOCK_DGRAM, 0);
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(PORT);
    servaddr.sin_family=AF_INET;

    //binding ip address to the socket 
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    //receiving datagram from client now
    //client is ready to receive the data
    len=sizeof(clieaddr);
    int n =recvfrom(listenfd, data, sizeof(data), 0, (struct sockaddr*)&clieaddr, &len);
    data[n]='\0';
    puts(data);
    
    //opening file now 
    char *filename="Quiz2/quiz-2-cse231/para2.txt";
    FILE *fptr=fopen(filename, "r");
    /*while(fscanf(fptr, "%s", message)!=EOF){ //getting data
        sendto(listenfd, message, MAX, 0, (struct sockaddr *)&clieaddr, sizeof(clieaddr));
    }*/
    while(fgets(data,100, fptr)!=NULL){
        char *tok=strtok(data, " ");
        while(tok!=NULL){
            //int val;
            //strcpy(message.msgtext, tok);
            sendto(listenfd, message, MAX, 0, (struct sockaddr *)&clieaddr, sizeof(clieaddr));
            printf("Sent Data %s",tok);
            tok=strtok(NULL, " ");
        }
    }
    sendto(listenfd, "ACK_END_CHECK", MAX, 0, (struct sockaddr *)&clieaddr, sizeof(clieaddr));
    fclose(fptr);
    return 0;
}

