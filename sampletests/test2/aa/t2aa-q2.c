/*
You are running portsentry on your machine which detects probes at various ports
and will block the prober. You want a program which will report the attacks which oc-
cured after the last report. The last report was based on the file which is currently named
/var/log/ps.udp.1 while the current set of attacks (which includes the old attacks) is stored
in /var/log/ps.udp.0. Write a C program that will do a diff between these two files and
email root the results. This could be written on the command line as below (but don’t).
diff /var/log/ps.udp.1 /var/log/ps.udp.0 | mail -s Attack root
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

			// run the diff command on the two files 
			execlp("diff", "diff", "data1", "data2", NULL);

			// shouldn't get here
			perror("exec");
			exit(-1);
		default:
			// parent dups the read end of pipe to stdin
			dup2(fd[0], STDIN_FILENO);

			// close unused ends of pipe
			close(fd[0]);
			close(fd[1]);

			// run mail command on output
			execlp("./mail", "mail", "-s", "Attack", "root", NULL);

			// shouldn't get here
			perror("exec");
			exit(-1);
	}

	exit(-1);

}