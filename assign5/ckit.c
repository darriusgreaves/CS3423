#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFSZ 1024

// function prototypes
void readFromFile(int argc, char * argv[], int fc);
void readFromStdin(int argc, char * argv[], int fc);
int getLength(char *fh);

// Low-level file IO
// Reads a list of files given on the commandline
// Prints contents of files to STDOUT or to output file (if given)
int main (int argc, char * argv[])
{
    int fdOut;
    int fc;
    int n;
    int m;
    // check usage
    if ( argc < 3 ) {
        fprintf(stderr,"Usage: prog1 <in file  or - > <in file or - > [out file]\n");
        exit(-1);
    }
    
    // check that there is at least one file listed in argv
    n = getLength(argv[1]);
    m = getLength(argv[2]);
    if(n == 1 && m == 1 && argv[1][0] == '-' && argv[2][0] == '-'){
                fprintf(stderr, "Input must contain at least one file\n");
                exit(-1);
            }    
    
    // Open the output file (if given on cmd line) - if it exists, truncate
    if( argc == 4){
        if((fdOut = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0){
            perror(argv[3]);
        }
        close(fdOut);
    }
    
    // loop through given input files
    fc = 1;
    while(fc < 3){
        if(argv[fc][0] == '-'){
            readFromStdin(argc, argv, fc);
        }
        else{
            readFromFile(argc, argv, fc);
        }
        
        fc++;
    }

    exit(0);
}

// Gets the length of a char pointer
int getLength(char * fh){
 int count = 0;
 while(*fh != '\0'){
     count++;
     fh++;
 }
 return count;
}


// This will open a file and write its contents to the 
// given output file or to the console.
void readFromFile(int argc, char * argv[], int fc){
    int n;
    int fdIn;
    int fdOut;
    char buf[BUFSZ];
    if((fdIn = open(argv[fc], O_RDONLY)) < 0 ) {
              perror(argv[fc]);
              exit(-1);
            }
    while ( (n = read(fdIn, buf, BUFSZ)) > 0 ) {
        if( argc == 4){
            fdOut = open(argv[3], O_WRONLY | O_APPEND);
            write(fdOut, buf, n);
            close(fdOut);
        }
        else{
            write(STDOUT_FILENO, buf, n);
        }
    }
    close(fdIn);
    
    if ( n < 0 ) {
      perror("I/O error");
      exit(-1);
    }
 }
 
 // This will read input fron stdin and write its contents to the 
// given output file or to the console.
 void readFromStdin(int argc, char * argv[], int fc){
    int n;
    int fdIn;
    int fdOut;
    char buf[BUFSZ];

    while ( (n = read(STDIN_FILENO, buf, BUFSZ)) > 0 ) {
        if( argc == 4){
            fdOut = open(argv[3], O_WRONLY | O_APPEND);
            write(fdOut, buf, n);
            close(fdOut);
        }
        else{
            write(STDOUT_FILENO, buf, n);
        }
    }

    if ( n < 0 ) {
      perror("I/O error");
      exit(-1);
    }
 }
 

