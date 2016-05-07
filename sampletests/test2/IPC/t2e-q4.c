/*
// Sample Test Test2e Question 4
//	Problem:
//		Write a C program which will fork 40 children.  Each child will send back to 
//		the parent process, via a single pipe, either one or two messages depending
//		on whether the child's pid is odd or even.  If the pid is even, the child
//		send the single message "one", while  a child with odd pid will send the 
//		message "one" followed by the message "two".  The parent process must 
//		not terminate until all messages have been read.
*/

#include <stdio.h>		// printf, perror, fflush 
#include <stdlib.h>		// exit, srand, rand
#include <unistd.h>		// (POSIX) fork, pipe, read, write, getpid, close
#include <sys/types.h>	// (POSIX) pid_t

#define NUM_CHILD	40

int main(void){

	int fd[2];		// pipe to communicate with
	int i;			// loop variable
	int indx;		// childs index
	int flag;		// for breaking out of fork-loop
	pid_t pid;		// pid of child
	char msg[4];	// message to send/recv (will be either "one" or "two")

	// set up pipe
	if(pipe(fd)){
		perror("pipe");
		exit(-1);
	}

	// fork the children
	for(i = 0; i < NUM_CHILD; i++){
		switch(flag = fork()){
			case -1:
				perror("fork");
				exit(-1);
			case 0:
				indx = i;
				break;
			default:
				break;
		}
		// child breaks out of fork-loop, parent keeps forking
		if(flag == 0){
			break;
		}
	}

	//if parent - read and print msgs, then exit
	if(flag){
		//close write end of pipe
		close(fd[1]);

		//read and print messages
		while(read(fd[0], &msg, sizeof(msg)) == sizeof(msg)){		
			printf("Message received: %s\n", msg);
			fflush(stdout);
		}

		// close read end and exit
		close(fd[0]);
		exit(0);
	}

	//otherwise child
	// close read end of pipe, and get pid
	close(fd[0]);
	pid = getpid();

	// if pid is even, write single message
	if(pid%2 == 0){
		snprintf(msg, 4, "one");
		if(write(fd[1], msg, sizeof(msg)) != sizeof(msg)){
			perror("write");
			exit(-1);
		}
	}
	else{
		// otherwise write two messages
		snprintf(msg, 4, "one");
		if(write(fd[1], msg, sizeof(msg)) != sizeof(msg)){
			perror("write");
			exit(-1);
		}
		snprintf(msg, 4, "two");
		if(write(fd[1], msg, sizeof(msg)) != sizeof(msg)){
			perror("write");
			exit(-1);
		}
	}

	//close write end and exit
	close(fd[1]);
	exit(0);	
}