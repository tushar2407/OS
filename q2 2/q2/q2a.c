// Your code here
// Your code here
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define BUFFER_SIZE 1024
// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
} message;
  
int main( int argc, char** argv ){
    printf("asd");
    key_t key;
    int msgid;
    key = ftok("q2c.c", 65);
    
      // msgget creates a message queue
      // and returns identifier
      msgid = msgget(key, 0666 | IPC_CREAT);
      message.mesg_type = 1;
    
      
    const char *delimiter_characters = " ";
    const char *filename = "para1.txt";
    FILE *input_file = fopen( filename, "r" );
    char buffer[ BUFFER_SIZE ];
    char *last_token;
	
    if( input_file == NULL ){

        fprintf( stderr, "Unable to open file %s\n", filename );

    }else{	
        // Read each line into the buffer
        while( fgets(buffer, BUFFER_SIZE, input_file)!=NULL ){

            // Write the line to stdout
            //fputs( buffer, stdout );
		
            // Gets each token as a string and prints it
            	last_token = strtok( buffer, delimiter_characters );
            while( last_token != NULL ){
                //last_token = strtok( NULL, delimiter_characters );
                strcpy(message.mesg_text, last_token);
                msgsnd(msgid, &message, sizeof(message), 0);
                printf("%s\n", last_token);          
		last_token = strtok( NULL, delimiter_characters );
		  
	}

//	fgets(buffer, BUFFER_SIZE, input_file) ;
        }

        if( ferror(input_file) ){
            perror( "The following error occurred" );
        }

        fclose( input_file );

    }

    return 0;

}
