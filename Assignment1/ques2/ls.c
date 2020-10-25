#include <dirent.h>
#include <stdio.h>
#include <string.h>
int main(int x, char **pointer){
    // printf("%s\n", (char*)pointer[0]);
    // printf("asdasd");
    DIR *d;
    struct dirent *dir;
    // printf("%s\n", pointer[0]);
    if(pointer[1]==NULL || (strcmp(pointer[1],"-r") || strcmp(pointer[1],"-a")))
        d = opendir(".");
    else
        d = opendir((const char*)pointer[1]);
    if(pointer[1]==NULL){
        // d = opendir((const char*)pointer[0]);
        if (d)
        {
            while ((dir = readdir(d)) != NULL)
            {
                // printf("%s ", strcmp(dir->d_name[0],"."));
                if(dir->d_name[0]!='.')
                    printf("%s\n", dir->d_name);
            }
            closedir(d);
        }
    }
    else if(strcmp(pointer[1], "-a")==0){
        if (d)
        {
            while ((dir = readdir(d)) != NULL)
            {
                // printf("%s ", strcmp(dir->d_name[0],"."));
                // if(dir->d_name[0]!='.')
                printf("%s\n", dir->d_name);
            }
            closedir(d);
        }   
    }
    
    else if(strcmp(pointer[1], "-r")==0){
        char names[500][1024];
        int length=0;
        if (d)
        {
            while ((dir = readdir(d)) != NULL)
            {
                // printf("%s ", strcmp(dir->d_name[0],"."));
                if(dir->d_name[0]!='.')
                    strcpy(names[length++], dir->d_name);
                    // printf("%s\n", dir->d_name);
            }
            for(int i=0;i<length;i++){
                for(int j=i+1;j<length;j++){
                    if(strcmp(names[i], names[j])<0){
                        char temp[1024];
                        strcpy(temp, names[i] );
                        strcpy(names[i], names[j] );
                        strcpy(names[j], temp );
                    }
                }
            }
            for(int i=0;i<length;i++){
                printf("%s\n",names[i] );
            }
            closedir(d);
        }
    }
    else if(pointer[1][0]=='-'){
        printf("Invalid flag input.\n");
    }
    else{
        printf("Invalid input.\n");
    }
    // printf("uweroqiubr\n");
    return 0;
}