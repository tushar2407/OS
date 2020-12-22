#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/file.h>
#include <string.h>

#define CTRL_KEY(k) ((k) & 0x1f) // detecting ctrl-Q

struct termios orig_termios;

void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() { // does not echo what we write on the terminal
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);

    struct termios raw = orig_termios; 
    raw.c_iflag &= ~(IXON | ICRNL);
    raw.c_lflag &= ~(ECHO | ICANON | ISIG); // enabling canonical mode to read data byte-by-byte not line-by-line
    // isig signal to turn off ctrl-C and ctrl-Z signals
    // IEXTEN disable ctrl-V
    // IXON disable ctrl-S(turn off transmission) + ctrl-Q (resume transmission)
    // ICRNL disable ctrl-M discrepency
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
    enableRawMode();
    char c;
    FILE *f;
    
    int fd;
    fd = open("tushar.txt",  O_CREAT | O_RDWR);
    
    printf("%s \n", strerror(errno));
    
    f = fdopen(fd, "a+");
    
    int status = flock(fd, LOCK_EX | LOCK_NB);

    // printf("%d \n", status);

    // printf("%s \n", strerror(errno));
    
    if(status==0);
    else if(errno == EWOULDBLOCK )
        printf("Warning !!!\n The file is already locked");
    else
        return 0;
    
    printf("Contents of the files: \n");
    char d = fgetc(f); 
    while (d != EOF) 
    { 
        printf ("%c", d); 
        d = fgetc(f); 
    } 
    printf("\nPress q to exit\n");
    printf("Give your inputs: \n");

    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
        if (iscntrl(c)) {
            if(c == CTRL_KEY('q'))
                printf("Control q was pressed`\n");
          printf("%d \n", c);
        } else {
          printf("%d ('%c')\n", c, c);
        }
        fprintf(f, "%c", c);
    }
    flock(fd, LOCK_UN);
    return 0;
}