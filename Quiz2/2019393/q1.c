/* 
    References:
    https://www.geeksforgeeks.org/pipe-system-call/
    https://www.geeksforgeeks.org/c-program-demonstrate-fork-and-pipe/
*/
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
int main()
{
    int pipe1[2], pipe2[2];
    if(pipe(pipe1)==1 || pipe(pipe2)==1)
    {
        printf("Unable to pipe\n");
        return 1;
    }
    char input[500];
    pid_t pid = fork();
    if(pid<0)
    {
        printf("Fork unsuccessful\n");
        return 0;
    }
    else if (pid==0)
    {
        // child process
        close(pipe1[1]);
        read(pipe1[0], input, 500);
        printf("Child string read : %s\n", input);
        char *mod=input;
        int i=0;
        for( i=0; i<strlen(input); i++){
            if(input[i]=='\\'){
                i++;
            }
            else{
                mod[i]=(char)(input[i]-32);
            }
        }
        write(pipe2[1], mod, strlen(mod)+1);
        close(pipe2[1]);
        close(pipe2[0]);
        close(pipe1[0]);
    }
    else
    {
        scanf("%s", input);
        close(pipe1[0]);
        write(pipe1[1],input, strlen(input)+1);
        close(pipe1[1]);
        wait(NULL);
        close(pipe2[1]);
        read(pipe2[0], input, 500); 
        printf("Modified string : %s\n", input);
        close(pipe2[0]);
    }
    return 0;
}