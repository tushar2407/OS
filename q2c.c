#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<ctype.h>
#include<unistd.h>
#include<sys/msg.h>
#include<sys/ipc.h>
#define one 1
#define two 2
#define len 100
#define fl 500

struct msgbuffer{
    long msgtype;
    char msgtext[100];
}
message ;

int main(){
    key_t key;
    int msgid;
    int flag=0666;
    key=ftok("progfile", 65);
    //creating message queue 
    msgid=msgget(key, 0666 | IPC_CREAT);
    //msgrcv for receiving message 

    while(1){ //since receiving never ends 
        if(msgrcv(msgid, &message, sizeof(message), 0, 0)==-1){
            perror("msgrcv");
            exit(1);
        }
        //displaying message on console 
        printf("%s", message.msgtext );
    }
    //destroy message queue
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}
