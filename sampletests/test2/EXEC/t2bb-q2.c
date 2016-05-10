/*
There are constant attacks on your machine consisting of thousands of ssh login attempts.
 This has no chance of working since you don’t use word-based passwords on your system but
 they irritate you and fill up your log files making it next to impossible to read your logs. You
 have written a program, sshreject whitelist which will read from stdin and if it finds 5
 failed login attempts without a success from the same IP (not in the file whitelist) will reject
 the route. Thus your machine becomes a “black hole” to that IP. However your program has
 to read the log file /var/log/auth.log which is periodically rotated. The tail program can
 be told to ignore the rotation and continue reading from the new auth.log file. Thus this
 could be done on the command line with the command:
tail -F /var/log/auth.log | sshreject myips
Write a C program that will implement the above
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
			execlp("tail", "tail", "-F", "data1", NULL);

			// shouldn't get here
			perror("exec");
			exit(-1);
		default:
			// parent dups the read end of pipe to stdin
			dup2(fd[0], STDIN_FILENO);

			// close unused ends of pipe
			close(fd[0]);
			close(fd[1]);

			// run sshreject command on output
			execlp("./sshreject", "sshreject", "myips", NULL);

			// shouldn't get here
			perror("exec");
			exit(-1);
	}
	exit(-1);
}