/*
// Sample Test Test2aa Question 3
//	Problem:
//		Write a C program which will create a total of 40 processes all of which will
//		have their own seperate pipe, i.e. all processes must be able to write to all
//		other processes but each will only read from its own pipe.  Each process will
//		be assigned a unique index 0, 1, ..., 39.  Each process with an odd pid will
//		write its index and pid to process 0 (possibly including process 0).  The even
//		pids will write their index and pid to all other processes (but not to itself).   
//		The processes with even pids will not write any messages.  Then each process 
//		(odd and even) will read all the messages written to its pipe and print on stdout, 
//		for each message, a line similar to:
//			Process 35894 index 24 heard from process 35993 index 16
//		Be careful that your code doesn't hang! 
*/

#include <stdio.h>		// printf, perror, fflush 
#include <stdlib.h>		// exit, 
#include <unistd.h>		// (POSIX) fork, pipe, read, write, getpid, close
#include <sys/types.h>	// (POSIX) pid_t

#define NUM_PROCS   40  // Number of processes to create

int main(void){
	
	int i;		        		// loop variables
	int pipes[NUM_PROCS][2];	// pipes for all the processes
	int idx;                    // process index
	int parent;                 // flag used to exit fork loop
	pid_t pidTemp;              // temp pid

	// message struct used to send and recieve messages
	struct message {
		pid_t pid;
		int index;
	} msg;

	// Set up the pipes
	for(i = 0; i < NUM_PROCS; i++){
		if(pipe(pipes[i]) == -1) {
			perror("pipe");
			exit(-1);
	}	}

	// fork all the processes
	for(i = 0; i < NUM_PROCS; i++){
		switch(parent = fork()){
			case -1:
				perror("fork");
				exit(-1);
			case 0:
				idx = i;
				parent = 0;
				break;
			default:
				parent = 1;
				break;
		}

		// children break out of fork loop, parent keeps forking
		if(parent == 0){
			break;
		}
	}

	// parent done forking can exit
	if(parent == 1){
		exit(0);
	}
	
	// child closes read end of all pipes except its own
	for(i = 0; i < NUM_PROCS; i++){
		if(idx != i){
			close(pipes[i][0]);
	}	}

	// child gets its proces id from kernel
	pidTemp = getpid();

	// set up message to send
	msg.pid = pidTemp;
	msg.index = idx;

	// if child is odd, write to proc 0
	if(pidTemp%2){
		if(write(pipes[0][1], &msg, sizeof(msg)) != sizeof(msg)){
			perror("write");
			exit(-1);
	}	}

	// If process id is even, child writes its messages
	else{
		for(i=0; i < NUM_PROCS; i++){
			if(idx != i){
				if(write(pipes[i][1], &msg, sizeof(msg)) != sizeof(msg)){
					perror("write");
					exit(-1);
	}	}	}	}

	// child closes write ends of process pipes
	for(i = 0; i < NUM_PROCS; i++){
		close(pipes[i][1]);
	}

	// child then stays up all night reading 
	while(read(pipes[idx][0], &msg, sizeof(msg)) == sizeof(msg)){		
		printf("Process %d index %d heard from process %d index %d\n", getpid(), idx, msg.pid, msg.index);
		fflush(stdout);
	}

	// child closes its read end and exits
	close(pipes[idx][0]);
	exit(0);	
}
