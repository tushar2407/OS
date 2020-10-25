#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
int main(int x, char** pointer){
    // printf(" %s\n", pointer[1]);
    struct tm tm = *localtime(&(time_t){time(NULL)});
    if(pointer[1]!=NULL &&  !strcmp(pointer[1], "-u")){
        tm =*gmtime(&(time_t){time(NULL)});
        // printf("%s +0530", asctime(&tm));
        char* time_array;
        time_array=strtok(asctime(&tm), " ");
        while(time_array!=NULL){
            char arr[100];
            strcpy(arr,time_array);
            int length=strlen(arr);
            if(arr[length-1]=='\n'){
                printf("UTC 2020\n");
            }
            else
                printf("%s ", arr);
            time_array=strtok(NULL, " ");
        }
    }
    else if(pointer[1]!=NULL && !strcmp(pointer[1], "-R")){
        char* arr[10];
        char* time_array;
        time_array=strtok(asctime(&tm), " ");
        int i=0;
        while(time_array!=NULL){
            arr[i]=time_array;
            // printf("%s\n", arr[i]);
            time_array=strtok(NULL, " ");
            i++;
        }
        printf("%s, %s %s 2020 %s +0530\n", arr[0], arr[2], arr[1], arr[3]);
    }
    else if(pointer[1]!=NULL && pointer[1][0]=='-'){
        printf("Invalid flag\n");
    }
    else{
        char* arr[10];
        char* time_array;
        time_array=strtok(asctime(&tm), " ");
        int i=0;
        while(time_array!=NULL){
            arr[i]=time_array;
            // printf("%s\n", arr[i]);
            time_array=strtok(NULL, " ");
            i++;
        }
        printf("%s %s %s %s IST 2020\n",arr[0], arr[1], arr[2], arr[3]);
    }
    return 0;
}