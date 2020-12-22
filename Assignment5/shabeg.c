/* Name: Shabeg Singh Gill
   Roll_Number: 2019388 */
#include<sys/file.h>
#include <errno.h>
#include<string.h>
#include<stdio.h>  
#include <stdlib.h>
int main( )  
{  
FILE *fp ;  
char ch ;  
int fd;
char sentence[1000];
 char *s,*p;

  s = malloc(256);
//   scanf("%255s", s); // Don't read more than 255 chars
//   printf("%s", s);
fd = open("shab.txt", O_RDWR | O_APPEND );
//printf("%d", fd);
printf("%s", strerror(errno));
fp = fdopen(fd, "a+");
int st = flock(fd, LOCK_EX | LOCK_NB);
printf("%s", strerror(errno));
if(st!=0)
{
    printf("Warning\n");
}
// char ch;
    while ( 1 )  
    {  
        ch = fgetc ( fp ) ;  
        if ( ch == EOF )  
        break ;  
        printf("%c",ch) ;  
    }  
    printf("Input a line  to be added to the file : \n");
    
    scanf("%s", s);
    while(s[0]!='q'){
        scanf("%s", s);
        // fgets(s, sizeof(s), stdin);
        fprintf(fp, "%s\n", s);
    }
    fclose(fp);  
    

return 0;
}