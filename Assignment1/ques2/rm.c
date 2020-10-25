#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>

int main(int x, char** params){
    if(params[1]==NULL){
        printf("Not enough params\n");
        return 0;
    }
    if(params[2]==NULL){
        
        FILE *fptr= fopen(params[1], "r");
        struct stat buf;
        stat(params[1], &buf);
        if(!fptr || !S_ISREG(buf.st_mode)){
            printf("File not found\n");
        }
        else if(S_ISREG(buf.st_mode)){
            remove(params[1]);
            // printf("Removed\n");
        }
        else{
            printf("File not found\n");
        }
    }
    else if(params[1][0]!='-'){
        int i=1;
        while(params[i]!=NULL){
            FILE *fptr= fopen(params[i], "r");
            struct stat buf;
            stat(params[i], &buf);
            if(!fptr || !S_ISREG(buf.st_mode)){
                printf("File not found\n");
            }
            else if(S_ISREG(buf.st_mode)){ 
                remove(params[i]);
                // printf("Removed\n");
            }
            else {
                printf("File not found\n");
            }
            i++;
        }
    }
    else if(!strcmp(params[1], "-v")){
        int i=2;
        while(params[i]!=NULL){
            struct stat buf;
            stat(params[i], &buf);
            FILE *fptr= fopen(params[i], "r");
            if(!fptr || !S_ISREG(buf.st_mode)){
                printf("File not found\n");
            }
            else if(S_ISREG(buf.st_mode)){
                remove(params[i]);
                printf("Removed %s\n", params[i]);
            }
            else{
                printf("File not found \n");
            }
            i++;
        }
    }
    else if(!strcmp(params[1], "-d")){
        int i=2;
        while(params[i]!=NULL){
            printf("%s\n", params[i]);
            int flag = remove(params[i]);
            if(flag){
                printf("Directory not found\n");
            }
            i++;
        }
    }
    else if(params[1][0]=='-'){
        printf("Invalid flag\n");
    }
    else{
        printf("Not enough params\n");
    }
    return 0;
}