#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

//function for reading the csv file by child process.
void csvFileReaderForChildProcess(FILE* f,char stId[100],char section[100],char n1[100],char n2[100],char n3[100],char n4[100]){
	
    //for reading through 202 lines of csv file. 202 line of csv file are of section A record.
    for(int i=1;i<202;i++){
    //first line will not be readed from the csv file
    // skip the first line of csv file.
    if(i>1){
    //starting from second line.
    //reading student id from each line of csv file.    
    fscanf(f, "%s", stId);
    //converting student id string into integer
    int studentid = atoi(stId);
    //reading section from each line of csv file
    fscanf(f, "%s", section); 
    //reading Assignment1 from each line of csv file.
    fscanf(f, "%s", n1);
    //converting Assignment1 string into integer
    int number1 = atoi(n1);    
    //reading Assignment2 from each line of csv file.
    fscanf(f, "%s", n2);
    //converting Assignment2 string into integer
    int number2 = atoi(n2);    
    //reading Assignment3 from each line of csv file.
    fscanf(f, "%s", n3);    
    //converting Assignment3 string into integer    
    int number3 = atoi(n3);
    //reading Assignment4 from each line of csv file.        
    fscanf(f, "%s", n4);
    //converting Assignment4 string into integer
    int number4 = atoi(n4);
    //caculating the average of all assignment marks.
    double average = (number1+number2+number3+number4)/4;
    //printing it into the screen.
    printf("  %s              %s            %d            %d             %d             %d          %f\n", stId,section,number1,number2,number3,number4,average);
    }
    else{
    //this else block is for first line reading
    //because we don't need first line so we will only read it through scanf but will not print on the consolel.
    fscanf(f, "%s", stId);
    fscanf(f, "%s", section);           
    fscanf(f, "%s", n1);           
    }    
    
    }
  
    fclose(f);

}


//function for reading the csv file by Parent process.
void csvFileReaderForParentProcess(FILE* f,char stId[100],char section[100],char n1[100],char n2[100],char n3[100],char n4[100]){
   
    for(int i=1;i<402;i++){
    if(i==1){
     //this if block is for first line reading
    //because we don't need first line so we will only read it through scanf but will not print on the consolel.
    fscanf(f, "%s", stId);
    fscanf(f, "%s", section);           
    fscanf(f, "%s", n1);           
    }
    else{
    //starting from second line.
    //reading student id from each line of csv file.    
    fscanf(f, "%s", stId);
    //converting student id string into integer
    int studentid = atoi(stId);
    //reading section from each line of csv file
    fscanf(f, "%s", section); 
    //reading Assignment1 from each line of csv file.
    fscanf(f, "%s", n1);
    //converting Assignment1 string into integer
    int number1 = atoi(n1);    
    //reading Assignment2 from each line of csv file.
    fscanf(f, "%s", n2);
    //converting Assignment2 string into integer
    int number2 = atoi(n2);    
    //reading Assignment3 from each line of csv file.
    fscanf(f, "%s", n3);    
    //converting Assignment3 string into integer    
    int number3 = atoi(n3);
    //reading Assignment4 from each line of csv file.        
    fscanf(f, "%s", n4);
    if(i>201){
    //we will reading all line of csv file but printing only lines of section B students Information on the console. 
    //converting Assignment4 string into integer
    int number4 = atoi(n4);
    //caculating the average of all assignment marks.
    double average = (number1+number2+number3+number4)/4;
    //printing it into the screen.
    printf("  %s              %s            %d            %d             %d             %d          %f\n", stId,section,number1,number2,number3,number4,average);
    }

        
    }
    }
  
    fclose(f);

}

int main(int argc, char const *argv[])
{

    //making the pid_t variable
    pid_t childPID,w;
    int status;
    // calling fork() system call.
    childPID = fork();
    char stId[100];
    char section[100];
    char n1[100];
    char n2[100];
    char n3[100];
    char n4[100];
    //opening file
    FILE* f = fopen("file.csv", "r"); 
    //checkin either childPID is valid or not
    if(childPID >= 0) 
    {
        if(childPID == 0)  //Code executed by child 
        {
             printf("Child Process Reading The File\n");
             printf("Student ID  Section Number  Assignment-1  Assignment-2  Assignment-3  Assignment-4  Average Marks\n");
             //calling function which is done by child process.
	    csvFileReaderForChildProcess(f,stId,section,n1,n2,n3,n4);   
	     printf("\n");	     
	     //exit system call.
	    exit(0);
            

	   
        }
        else if(childPID>1) //Code executed by Parent
        {
           //waiting the parent process by waitpid() system call
	    w = waitpid(childPID, &status, WUNTRACED | WCONTINUED);
	    //checking through which intereption status is changed.
            if (w == -1) { perror("waitpid"); exit(EXIT_FAILURE); }


            if (WIFEXITED(status)) {
                printf("exited, status=%d\n", WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                printf("killed by signal %d\n", WTERMSIG(status));
            } else if (WIFSTOPPED(status)) {
                printf("stopped by signal %d\n", WSTOPSIG(status));
            } else if (WIFCONTINUED(status)) {
                printf("continued\n");
            }
            
	   printf("Child Complete\n");
	    printf("Parent Process Reading The File\n\n");
	    //calling function of parent.
	    csvFileReaderForParentProcess(f,stId,section,n1,n2,n3,n4);
        }
    }
    else // fork failed
    {
        fprintf(stderr, "Fork Failed");
        //return 1;
    }
    return 0;


}