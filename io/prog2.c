#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFSZ 1024


// Low-level file IO
// Reads a list of files given on the commandline
// Prints contents of files to STDOUT
int main (int argc, char * argv[])
{
    int n;
    int fd;
    int fc;
    char buf[BUFSZ];

    if ( argc < 2 ) {
    fprintf(stderr,"Usage: prog1 <file list>\n");
    exit(-1);
    }
        
    fc = 1;
    while(fc < argc){
        if((fd = open(argv[fc], O_RDONLY)) < 0 ) {
          perror(argv[fc]);
          exit(-1);
        }

        while ( (n = read(fd, buf, BUFSZ)) > 0 ) {
          write(STDOUT_FILENO,buf,n);
        }

        if ( n < 0 ) {
          perror("I/O error");
          exit(-1);
        }
        fc++;
    }

    exit(0);
}

