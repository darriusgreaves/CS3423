/*
You are running snort on your machine but the amount of data is staggering so you want
 a simple program which you can run in a window that will only report probes at port 111 on
 your machine. This could be done with the command-line pipe:
tail -f /var/portscan.log | egrep 129.115.29.203:111
but you want a single program. Write a C program which will perform this functionality.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>

int main(int argc, char *argv[]){
	int fd[2];		// pipe to comunicate with

	// open pipe
	if(pipe(fd)){
		perror("pipe");
		exit(-1);
	}

	// fork, dup, exec
	switch(fork()){
		case -1:
			perror("fork");
			exit(-1);
		case 0:
			// child dups write end of pipe to stdout
			dup2(fd[1], STDOUT_FILENO);

			// close unused ends of pipes
			close(fd[0]);
			close(fd[1]);

			// run the tail command on the file data1
			execlp("tail", "tail", "-f", "data1", NULL);

			// shouldn't get here
			perror("exec");
			exit(-1);
		default:
			// parent dups the read end of pipe to stdin
			dup2(fd[0], STDIN_FILENO);

			// close unused ends of pipe
			close(fd[0]);
			close(fd[1]);

			// run egrep command on output
			execlp("egrep", "egrep", "207.238.228.11", NULL);

			// shouldn't get here
			perror("exec");
			exit(-1);
	}
	exit(-1);
}