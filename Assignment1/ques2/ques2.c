#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
// extern void ls(char*);
void handle_rm(char** params, int length, char* env_var ){
    pid_t pid = fork();
    if(pid==0){
        // char temp[]=strdup(env_var);
        char *temp=malloc(1000*sizeof(char));
        strcpy(temp, env_var);
        strcat(temp, "/rm");
        printf("%d", execvp(temp,params));
        // printf("%d", execvp("./rm",params));
    }
    else{
        wait(NULL);
    }
}

void handle_cat(char** params, int length, char* env_var){
    pid_t pid = fork();
    if(pid==0){
        // char temp[]=strdup(env_var);
        char *temp=malloc(1000*sizeof(char));
        strcpy(temp, env_var);
        strcat(temp, "/cat");
        printf("%d", execvp(temp,params));
        // fptr = fopen(temp, "w");
        // printf("%d", execvp("./cat",params));
    }
    else{
        wait(NULL);
    }
}

void add_to_history(char* params, int length, char* env_var){
    // char temp[]=strdup(env_var);
    char *temp=malloc(1000*sizeof(char));
    strcpy(temp, env_var);
    strcat(temp, "/history.txt");
    // printf("%d", execvp(temp,params));
    // fptr = fopen(temp, "w");
    FILE *file = fopen(temp, "a");
    fprintf(file, "%s", params);
    fclose(file);
}

void handle_history(char** params, int length, char* env_var){ // history is f
    FILE *file, *fptr;
    // printf("asads");
    if(params[1]==NULL){
        // char temp[]=strdup(env_var);
        char *temp=malloc(1000*sizeof(char));
        strcpy(temp, env_var);
        strcat(temp, "/history.txt");
        // printf("%d", execvp(temp,params));
        file = fopen(temp, "r");
        char line[1024];
        int i=1;
        while(fgets(line, 4096, file)!=NULL){
            printf("%d %s", i++, line);
        }
        fclose(file);
    }
    else if(!strcmp(params[1], "-c")){
        // char temp[]=strdup(env_var);
        char *temp=malloc(1000*sizeof(char));
        strcpy(temp, env_var);
        strcat(temp, "/history.txt");
        // execvp(temp,params);
        fptr = fopen(temp, "w");
        fclose(fptr);
    }
    else if(!strcmp(params[1], "-w") && length==3){
        // char temp[]=strdup(env_var);
        char *temp=malloc(1000*sizeof(char));
        strcpy(temp, env_var);
        strcat(temp, "/history.txt");
        // execvp(temp,params);
        fptr = fopen(temp, "r");
        // file = fopen("history.txt", "r");
        FILE *fptr = fopen(params[2], "w");
        char line[1024];
        // int i=1;
        while(fgets(line, 4096, file)!=NULL){
            fprintf(fptr, "%s", line );
        }
        fclose(fptr);
        fclose(file);
    }
    
}

void handle_mkdir(char** params, int length, char* env_var){
    pid_t pid = fork();
    if(pid==0){
        // printf("asdas");
        // char temp[]=strdup(env_var);
        char *temp=malloc(1000*sizeof(char));
        strcpy(temp, env_var);
        strcat(temp, "/mkdir");
        execvp(temp,params);
    }
    else{
        wait(NULL);
    }
}

void handle_ls(char** params, int length, char* env_var){
    pid_t pid =fork();
    if(pid==0){
        // char str[1024]; 
        // getcwd(str, 1024);
        // char temp[]=strdup(env_var);
        char *temp=malloc(1000*sizeof(char));
        strcpy(temp, env_var);
        strcat(temp, "/ls");
        execvp(temp,params);
        // printf("%s\n", params[0]);
        // printf("%d", execvp("./ls",params));   
    }
    else{
        wait(NULL); 
        // printf("asdasd");
    }
}
void handle_echo(char** params, int length, char* env_var){
    if(params[1]==NULL){
        printf("\n");
    }
    else if(!strcmp(params[1],"--help")){
        // char temp[]=strdup(env_var);
        char *temp=malloc(1000*sizeof(char));
        strcpy(temp, env_var);
        strcat(temp, "/echo.txt");
        FILE *fptr= fopen(temp, "r");
        char line[1000];
        while(fgets(line,1000,fptr )!=NULL){
            printf("%s",line );
        }
        printf("\n");
    }
    else if(!strcmp(params[1],"-n")){
        int j=2;
        while(params[j]!=NULL){
            char str[1024];
            strcpy(str, params[j]);
            int i=0;
            while (i<strlen(str))
            {
                if(str[i]!='"')
                    printf("%c", str[i]);
                // ptr++;
                i++;
            }
            printf(" ");
            j++;
        }
    }
    else{
        int j=1;
        while(params[j]!=NULL){
            char str[1024];
            strcpy(str, params[j]);
            int i=0;
            while (i<strlen(str))
            {
                if(str[i]!='"')
                    printf("%c", str[i]);
                // ptr++;
                i++;
            }
            j++;
        }
        printf("\n");
    }

}
void handle_pwd(char** params, int length, char* env_var){
    if(params[1]==NULL){
        printf("%s\n", env_var);
    }
    else if(!strcmp(params[1], "-L")){
        printf("%s\n", env_var);
    }
    else if(!strcmp(params[1], "-P")){
        char dir[1000];
        getcwd(dir,1000);
        printf("%s\n",dir);
        // strcpy(env_var, dir);
    }
}
void handle_date(char** params, int length, char* env_var){
    pid_t pid = fork();
    if(pid==0) {
        char *temp=malloc(1000*sizeof(char));
        strcpy(temp, env_var);
        strcat(temp, "/date");
        printf("%d", execvp(temp,params));
        free(temp);
        // printf("%d", execvp("./date",params)); 
    }
    else {
        wait(NULL);
    }
}
void type_prompt(char env_var[1000]){
    static int first_time=1 ;
    if(first_time){
        // clear screen for the first time
        const char* CLEAR_SCREEN_ANSI = " \e[;1H\e[;2J";
        write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
        first_time=0;
    }
        printf("TusharMohan_2019393:%s$", env_var); 
}
int main(){
    char env_var[1000], permanent_env[1000];
    getcwd(env_var,1000);
    strcpy(permanent_env, env_var);
    char slash[2]={'/', '\0'};
    FILE *file;
    while(1){
        // file = fopen("history.txt", "a");
        type_prompt(env_var);
        char command[10], *splittedString[20];
        char *line = malloc(100*sizeof(char));
        int count=0;
        char *array[10];
        int flag=0;
        while(1)
        {
            int c=fgetc(stdin);
            line[count++]=(char)c;
            if(c=='\n')
            {
                flag=1;
                break;
            }
        }
        int i=0;
        add_to_history(line, 1, env_var);
        char *pch=strtok(line," \n");
        //printf("%p \n", &pch);
        while(pch!=NULL)
        {
            array[i++]=strdup(pch);
            //printf("%s", array[i]);
            pch=strtok(NULL," \n");
        }
        strcpy(command,array[0]);
        
        for(int j=0;j<i;j++)
        {
            splittedString[j]=array[j];
        }
        // printf("%s\n", command);
        // for(j=0;j<splittedString.length;j++){
        //     printf("%s\n", splittedString[j]);
        // }
        splittedString[i]=NULL;
        free(line);
        // printf("%s\n", env_var);
        // getcwd(env_var, 1000);  
        // printf("%s\n", env_var); 
        // printf("oboin");
        if(!strcmp(splittedString[0],"cd")){
            // int p=0; //flag
            char temp[1000];
            strcpy(temp, env_var);
            // // printf("asdas");
            if(i==1 || 
                (i==2 
                && (!strcmp(splittedString[1], "-P") 
                    || !strcmp(splittedString[1], "-L")))){
                char username[]="/home/";
                strcat(username,getenv("USERNAME"));
                chdir(username);
                getcwd(temp,1000);
                strcpy(env_var,temp);
                continue;
            }
            if(!strcmp(splittedString[1],"-P")){
                // p=1;
                if(chdir(splittedString[2])==0){
                    getcwd(env_var,1000);
                    // strcpy(env_var, splittedString[2]);
                }
                else{
                    printf("%s\n", strerror(errno));
                }
                continue;
            }
            // int flag=0;
            char cur_dir[1000];
            if(!strcmp(splittedString[1],"-L")){
                // flag=1;
                strcpy(cur_dir, splittedString[2]);
            }
            else{
                strcpy(cur_dir, splittedString[1]);
            }
            char *copy=malloc(1000*sizeof(char));
            strcpy(copy, env_var);
            if(cur_dir[0]=='/'){
                strcpy(env_var,  cur_dir);
            }
            else if(!strcmp(cur_dir,"..")){
                char copy2[1000];
                int k=strlen(env_var)-1;
                while(env_var[k--]!='/'){}
                for(int j=0; j<=k; j++){
                    copy2[j] = env_var[j];
                }
                copy2[k+1]='\0';
                strcpy(env_var,copy2);
            }
            else if(!strcmp(cur_dir,".")){
                continue;
            }
            else{
                strcat(env_var, slash);
                strcat(env_var, cur_dir);
            }
            
            if(chdir(env_var)==0){
                continue;
            }
            else{
                strcpy(env_var, copy);
                printf("%s\n", strerror(errno));
            }
        }
        else if(!strcmp(splittedString[0],"ls")){
            handle_ls(splittedString, i, permanent_env);
        }
        else if(!strcmp(splittedString[0],"history")){
            handle_history(splittedString, i, permanent_env);
        }
        else if(!strcmp(splittedString[0], "mkdir")){
            handle_mkdir(splittedString, i, permanent_env);
        }
        else if(!strcmp(splittedString[0], "cat")){
            handle_cat(splittedString, i, permanent_env);
        }
        else if(!strcmp(splittedString[0],"exit")){
            if(splittedString[1]==NULL)
                break;
            else{
                FILE *fptr = fopen("exit.txt", "r");
                char line[1000];
                while(fgets(line,1000, fptr)){
                    printf("%s", line);
                }
                printf("\n");
            }
        }
        else if(!strcmp(splittedString[0], "pwd")){
            handle_pwd(splittedString, i, permanent_env);
        }
        else if(!strcmp(splittedString[0], "echo")){
            handle_echo(splittedString, i, permanent_env);
        }
        else if(!strcmp(splittedString[0], "date")){
            handle_date(splittedString, i, permanent_env);
            // printf("Asda");
        }
        else if(!strcmp(splittedString[0], "rm")){
            handle_rm(splittedString, i, permanent_env);
        }   
        else {
            printf("Invalid command\n");
        }
    }
    // fclose(file);
    return 0;
}