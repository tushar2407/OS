// #include <stdio.h>
// #include <sys/types.h> 
// #include <sys/wait.h>
// #include <unistd.h> 
// #include <string.h>
// #include <stdlib.h>
 
// void read_command( char cmd[], char *par[]){
//     char line[1024];
//     int count = 0, i = 0, j = 0;
//     char *array[100], *pch;
//     for(;;){
//         int c = fgetc( stdin );
//         line[count++] = (char)c;
//         if ( c=='\n' ) break;
//     }
//     if( count == 1) return;
//     pch = strtok(line, " \n");
//     // parse line into words
//     while(pch != NULL){
//         array[i++] = strdup( pch );
//         pch = strtok( NULL, "\n");
//     }
//     // first word in the command
//     strcpy(cmd, array[0]);
//     // other are parameters
//     for(int j=0; j<i; j++){
//         par[j] = array[j];
//     }
//     par[i] = NULL;

// }
// void type_prompt(){
//     static int first_time=1;
//     if(first_time){
//         // clear screen for the first time
//         const char* CLEAR_SCREEN_ANSI = " \e[;1H\e[2J";
//         write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
//         first_time=0;
//     }
//     printf("#"); // display prompt
// }
// int main(){
//         char cmd[100], command[100], *parameters[20];
//         // environment variable
//         char *envp[] = {(char *) "PATH=/bin", 0};
//         while(1){
//             type_prompt();
//             read_command( command, parameters);
//             if( fork() != 0){
//                 wait(NULL);
//             }
//             else{
//                 strcpy( cmd, "/bin/");
//                 strcat( cmd, command);
//                 execve(cmd, parameters, envp);
//             }
//             if(strcmp( command, "exit")==0)
//                 break;
//         }
// }

/*
    ls
*/
// #include<stdio.h>
// #include<stdlib.h>
// #include<fcntl.h>
// #include<sys/stat.h>
// #include<dirent.h>
// #include <unistd.h>
// int main()
// {
//     // ls command
//     char *pointer=NULL;
//     DIR *dp=NULL;
//     struct dirent *sd=NULL;
//     pointer = getenv("PWD");
//     pointer="/home/tushar/how";
//     dp=opendir((const char*)pointer);
//     while((sd=readdir(dp))!=NULL)
//     {
//         printf("%s \t ",sd->d_name);
//     }
//     printf("\n");
//     // printf("%d", getcwd());
    
//     // getcwd below
//     long size;
//     char *buf;
//     char *ptr;

//     size = pathconf(".", _PC_PATH_MAX);

//     if ((buf = (char *)malloc((size_t)size)) != NULL)
//         ptr = getcwd(buf, (size_t)size);
//     printf("%s\n", ptr);
//     return 0;
// }



// #include <dirent.h>
// #include <stdio.h>
// void ls(char *pointer)
// {
//     DIR *d;
//     struct dirent *dir;
//     d = opendir((const char*)pointer);
//     if (d)
//     {
//         while ((dir = readdir(d)) != NULL)
//         {
//             printf("%s\n", dir->d_name);
//         }
//         closedir(d);
//     }
// }
// int main(int x, char **pointer){
//     // printf("asdas\n");
//     DIR *d;
//     struct dirent *dir;
//     d = opendir((const char*)pointer[0]);
//     if (d)
//     {
//         while ((dir = readdir(d)) != NULL)
//         {
//             printf("%s\n", dir->d_name);
//         }
//         closedir(d);
//     }
//     return 0;
// }


// #include <conio.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
  
void main(int x, char** pointer) 
{ 
    int check; 
    char* dirname = "geeskforgeeks"; 
    // clrscr(); 
    dirname=pointer[0];
    check = mkdir(dirname,0777); 
  
    // check if directory is created or not 
    if (!check) 
        printf("Directory created\n"); 
    else { 
        printf("Unable to create directory\n"); 
        exit(1); 
    } 
  
    // getch(); 
  
    system("dir"); 
    // getch(); 
} 