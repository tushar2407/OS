#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
int directoryExists(char directory[1024])
{
    DIR* d = opendir(directory);
    if(d)
    {
        closedir(d);
        return 1;
    }
    else if(ENOENT == errno)
    {
        return 0;
    }
    return 2;
}
int main(int x, char** pointer){
    // printf("%d", strcmp(pointer[3],"asda"));
    // if(strcmp)
    if(pointer[1]==NULL){
        printf("Insufficient arguments\n" );
        return 0;
    }
    int i=2;
    if(pointer[2]==NULL){
        if(pointer[1][0]=='/'){
            int check=mkdir(pointer[1],0777);
            if(check)
                printf("%s\n",strerror(errno) );
        }
        else{
            char *temp=strtok(pointer[1], "/");
            temp=strtok(NULL, "/");
            if(temp!=NULL){
                printf("You may want to add -P to do this\n");
            }
            else{
                if(pointer[1][0]=='-'){
                    printf("Invalid option \n");
                }
                else{
                    int check=mkdir(pointer[1], 0777);
                    if(check)
                        printf("%s\n",strerror(errno) );
                }
                // printf("%s\n", strerror(errno));
            }
        }
    }
    else{
        while(pointer[i]!=NULL){
            char temp_pointer[100], temporary[100];
            strcpy(temp_pointer, pointer[i]);
            strcpy(temporary, pointer[i]);
            char *dir=strtok(pointer[i], "/");
            char *directory=malloc(100 * sizeof(char));
            char *temp=strtok(temp_pointer, "/");
            if( pointer[1][0]=='-' &&
                strcmp(pointer[1], "-v") && strcmp(pointer[1], "-P")){
                printf("Invalid flag\n");
                return 0;
            }
            temp=strtok(NULL,"/");
            if(temp==NULL){
                // printf("qwoeubqwo");
                int check=mkdir(dir,0777);
                if(check){
                    printf("%s\n",strerror(errno));
                    return 0;
                }
                if(!strcmp(pointer[1],"-v")){
                    printf("mkdir: created directory '%s'", pointer[i]);
                }
            }
            else{ 
                if(temporary[0]=='/'){
                    // printf("oebrober\n");
                    char array[2]={'/', '\0'};
                    dir = strtok(pointer[i],"/");
                    strcat(directory, array);
                    strcat(directory, dir);
                    strcat(directory, array);
                    dir = strtok(NULL, "/");
                    while(directoryExists(directory)==1 && dir!=NULL){
                        strcat(directory, dir);
                        strcat(directory, array);
                        dir = strtok(NULL, "/");
                        printf("%s\n", dir);
                    }
                    int check=mkdir(directory,0777);
                    if(check==-1 ){
                        // printf("asddsa\n");
                        printf("mkdir: cannot create directory ‘%s’: No such file or directory\n", directory);
                    }
                    else{
                        strcat(directory, dir);
                        strcat(directory, array);
                        while(directoryExists(directory)!=1 && dir!=NULL){
                            check=mkdir(directory,0777);
                            // printf("%d\n", check);
                            strcat(directory, dir);
                            strcat(directory, array);
                            dir = strtok(NULL, "/");
                            // printf("%s\n", directory);
                        }   
                        // printf("asdaf");
                    }
                }
                else{
                    // printf("sdfas\n");
                    char array[2]={'/', '\0'};
                    // strcat(directory, array);
                    strcat(directory, dir);
                    strcat(directory, array);
                    dir = strtok(NULL, "/");
                    while(directoryExists(directory)==1 && dir!=NULL){
                        strcat(directory, dir);
                        strcat(directory, array);
                        dir = strtok(NULL, "/");
                    }
                    int check=mkdir(directory,0777);
                    if(check==-1 ){
                        printf("mkdir: cannot create directory ‘%s’: No such file or directory\n", pointer[2]);
                    }
                    else{
                        strcat(directory, dir);
                        strcat(directory, array);
                        while(directoryExists(directory)!=1 && dir!=NULL){
                            check=mkdir(directory,0777);
                            // printf("%d\n", check);
                            strcat(directory, dir);
                            strcat(directory, array);
                            dir = strtok(NULL, "/");
                            // printf("%s\n", directory);
                        }   
                    }
                }
            }
            i++;
        }
    }
    return 0;
}