#include <stdio.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <unistd.h> 
#include <string.h>
#include <stdlib.h>
 
int main(){
    FILE *file=fopen("file.csv", "r");
    pid_t id;
    id=fork();
    if(id<0){
        printf("Some error occured\n");
        return 1;
    }
    else if(id==0){
        char columns[78];
        fgets(columns, 78, (FILE*)file);
        // printf("%s",columns); 
        printf("Student ID Section Average\n");
        // columns done
        // now data left
        char row[16];
        char *ptr;
        while(fgets(row, 16, (FILE*)file)!=NULL){
            // printf("%s", row);
            // row read
            int i=0, flag=0;
            char row_copy[16];
            strcpy(row_copy,row);
            ptr = strtok(row, ",");
            // printf("%s", ptr);
            while( ptr != NULL ) {
                // printf( " %s\n", ptr ); //printing each token
                if(i==1 && !strcmp(ptr, "A")){
                    flag=1;
                    break;
                }
                ptr = strtok(NULL, ",");
                i++;
            }
            if(flag==1){
                i=0;
                float average=0.0;
                ptr=strtok(row_copy,",");
                while(ptr!=NULL){
                    if(i<=1)
                        printf("    %s", ptr);
                    else
                    {
                        average+=atoi(ptr);
                    }
                    ptr=strtok(NULL,",");
                    i++;
                }
                average/=4;
                printf("        %f\n", average);
            }
        }
        fclose(file);
        // child process done
        exit(0);
    }
    else{
        int status;
        waitpid(getpid()+1, NULL, 0);
        // waitpid(getpid()+1);
        file = fopen("file.csv", "r");
        char columns[78];
        fgets(columns, 78, (FILE*)file);
        printf("Parent Process \n");
        printf("Student ID Section Average\n"); 
        // printf("fasfa\n");
        // columns done
        // now data left
        char row[16];
        char *ptr;
        while(fgets(row, 16, (FILE*)file)!=NULL){
            int i=0;
            int flag=0;
            char row_copy[16];
            strcpy(row_copy,row);
            ptr = strtok(row, ",");
            // printf("%s", ptr);
            while( ptr != NULL ) {
                // printf( " %s\n", ptr ); //printing each token
                if(i==1 && !strcmp(ptr, "B")){
                    flag=1;
                    break;
                }
                ptr = strtok(NULL, ",");
                i++;
            }
            
            if(flag==1){
                i=0;
                float average=0.0;
                
                ptr=strtok(row_copy,",");
                while(ptr!=NULL){
                    if(i<=1)
                        printf("    %s ", ptr);
                    else
                    {
                        average+=atoi(ptr);
                    }
                    ptr=strtok(NULL,",");
                    i++;
                }
                average/=4;
                printf("      %f\n", average);
                
            }
        }
        //parent process done
    }
    fclose(file);
    // parent process
    return 0;
}