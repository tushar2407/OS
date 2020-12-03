// Your code here
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
} message;
  
#define BUFFER_SIZE 100
int main(){
    printf("asd");
    key_t key;
    int msgid;
    key = ftok("progfile", 65);
    
    //   // msgget creates a message queue
    //   // and returns identifier
    msgid = msgget(key, 0666 | IPC_CREAT);
    message.mesg_type = 1;
    
      
    const char *delimiter_characters = " ";
    const char *filename = "../../Quiz2/quiz-2-cse231/para1.txt";
    FILE *input_file = fopen( filename, "r" );
    char buffer[ BUFFER_SIZE ];
    char *last_token;
	
    if( input_file == NULL ){

        fprintf( stderr, "Unable to open file %s\n", filename );

    }
    else{
		
		printf("hello world\n");
        // Read each line into the buffer
        while(fgets(buffer, 100,input_file ) && input_file!=NULL)
        {
            char *tok = strtok(buffer, " ");
            while(tok){
                strcpy(message.mesg_text, tok);
                printf("%s\n", message.mesg_text);
                // msgsnd(msgid, &message, sizeof(message), 0);
                msgsnd(msgid, &message, sizeof(message), 0); 
                tok = strtok(NULL, " ");
            }
        }

    }

    return 0;

}

