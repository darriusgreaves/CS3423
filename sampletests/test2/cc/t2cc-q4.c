/*
Write a function with prototype
int pd2open(char *prog, char *argv[], int *fdread, int *fdwrite);
which will fork and exec the program with name, prog, and with the given argv array. Stdin
 and stdout of prog should be hooked to pipes with fdwrite being set to the write end of
 the stdin pipe and fdread being set to the read end of the stdout pipe. The function should
 return -1 on error and 0 otherwise.
 Basically the main program wants to execute prog and write data to prog (via fdwrite) and
 read the output from prog from the variable fdread. In particular pd2open returns two file
descriptors by putting them in the variables whose addresses are passed to the function.
*/

#include <stdio.h>		// printf, perror, fflush 
#include <stdlib.h>		// exit, 
#include <unistd.h>		// (POSIX) fork, pipe, read, write, getpid, close
#include <sys/types.h>	// (POSIX) pid_t

int pd2open(char *prog, char *argv[], int *fdread, int *fdwrite){
	int fd1[2], fd2[2];		// need two pipes to create "bi-directional" effect

	// open pipes
	if(pipe(fd1) || pipe(fd2)){
		perror("pipes");
		exit(-1);
	}
	// fork, dup, exec
	switch(fork()){
		case -1:
			perror("fork");
			exit(-1);
		case 0:
			// hook up the read end to stdin and close the rest
			dup2(fd1[0], STDIN_FILENO);
			close(fd1[0]);
			close(fd1[1]);

			// hook up the write end to stdout and close the rest
			dup2(fd2[1], STDOUT_FILENO);
			close(fd2[0]);
			close(fd2[1]);
			
			// exec the program
			execvp(prog, argv);

			// catch exec errors
			perror("exec");
			exit(-1);
	}

	// parent
	*fdread = fd2[0];	// hook caller up to prog read end of pipe
	*fdwrite = fd1[1];	// hook caller up to prog write end of pipe
	close(fd2[1]);		// close remaining pipes
	close(fd1[0]);
	return 0;
}
