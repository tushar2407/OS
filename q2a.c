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
    char msgtext[100] ;
}
message;

int main(){

    key_t key;
    int msgid;
    //generating key 
    key=ftok("progfile", 65);
    int flag=0666;

    //creating message queue 
    msgid=msgget(key,0666 | IPC_CREAT );
    message.msgtype=1;

    char data[1000];
    FILE *fptr = fopen("Quiz2/quiz-2-cse231/para1.txt", "r");
    //fgets(line,sizeof(line), fptr); //Skip Header
    //const char s[3] = " ";
    //char *token;
    while(fgets(data, 1000, fptr)!=NULL){
        char *tok=strtok(data, " ");
        while(tok!=NULL){
            int val;
            strcpy(message.msgtext, tok);
            val=msgsnd(msgid, &message, sizeof(message), 0);
            tok = strtok(NULL, " ");
        }
    }
    fclose(fptr);
    return 0;

}



    