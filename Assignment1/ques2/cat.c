#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
int main(int x, char** params){
    // printf("%s\n", params[1]);
    if(params[1]==NULL){
        char line[1024];
        while(fgets(line,1024,stdin)!=0){
            printf("%s", line);
        }
    }
    else if(!strcmp(params[1],"-E")){
        if(params[2]==NULL){
            char line[1024];
            while(fgets(line,1024,stdin)!=0){
                for(int i=0;i<strlen(line)-1;i++)
                    printf("%c", line[i]);
                printf("$\n");
            }
        }
        else{
            // printf("hahaa");
            FILE *fptr;
            fptr = fopen(params[2], "r");
            if(fptr==NULL){
                printf("%s\n", strerror(errno));
                return 0;
            }
            int i=1;
            char line[4096];
            while(fgets(line, 4096, fptr)!=NULL){
                for(int i=0;i<strlen(line)-1;i++)
                    printf("%c", line[i]);
                printf("$\n");
                // printf("%d %s", i++, line);
            }
            fclose(fptr);
        }
    }

    else if(!strcmp(params[1],"-n")){
        if(params[2]==NULL){
            char line[1024];
            int i=1;
            while(fgets(line,1024,stdin)!=0){
                printf("%d %s", i++, line);
            }
            // printf("$\n");
        }
        else{
            // printf("hehee");
            FILE *fptr;
            fptr = fopen(params[2], "r");
            if(fptr==NULL){
                printf("%s\n", strerror(errno));
                return 0;
            }
            int i=1;
            char line[4096];
            while(fgets(line, 4096, fptr)!=NULL){
                printf("%d %s", i++, line);
            }
            fclose(fptr);
        }
    }
    else if(params[1][0]=='-'){
        // printf("%s\n", params[1]);
        printf("Invalid argument\n");
    }
    else{
        // printf("erwe");
        FILE *fptr;
        fptr=fopen(params[1], "r");
        if(fptr==NULL){
            printf("%s\n", strerror(errno));
        }
        else{
            char line[1024];
            while(fgets(line, 1024, fptr)!=NULL){
                printf("%s", line);
            }
            
            fclose(fptr);
        }
        // FILE *fptr;
        // fptr = fopen("ques3", "r");
        // int i=0;
        // char line[4096];
        // while(fgets(line, 4096, fptr)!=NULL){
        //     printf(" %s", line);
        //     i=1;
        // }
        // fclose(fptr);
        // if(i==0)
        //     printf("asdasdasd");
        // else
        // {
        //     printf("brtbrtb");
        // }
        
    }
    // printf("%s\n", params[1]);

    return 0;
}