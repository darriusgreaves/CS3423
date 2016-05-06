#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFSZ 1024


// Low-level file IO
// Reads a file given on the commandline
// Prints contents of file to STDOUT
int main (int argc, char * argv[])
{
 int n;
 int fd;  // the file descriptor index number
 char buf[BUFSZ];

 if ( argc != 2 ) {
   fprintf(stderr,"Usage: prog1 <file>\n");
   exit(-1);
 }

if((fd = open(argv[1], O_RDONLY)) < 0 ) {
  perror(argv[1]);
  exit(-1);
}

while ( (n = read(fd, buf, BUFSZ)) > 0 ) {
  write(STDOUT_FILENO,buf,n);
}

if ( n < 0 ) {
  perror("I/O error");
  exit(-1);
}

exit(0);
}

