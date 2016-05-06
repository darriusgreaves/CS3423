// from Spring 2009 Final question 7
// Write a C program which will fork num (where num is a command-line argument)
// children. Every child with an odd pid (but not the original parent) will write his pid to all
// the other children (but not parent). Every child will then read every message written to him,
// one at a time, and then write each received pid along with his pid to the original parent using
// the following message structure.
// struct message {
// 	pid_t my_pid;
// 	pid_t received_pid;
// }
// On receipt of each message, the parent process will print the received data to stdout in the
// following form:
// Process 1234 received a message from Process 4567
// You do not need to worry about pipes filling.


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

// message struct used to send and recieve messages
struct message {
	pid_t my_pid;
	pid_t received_pid;
};

int main(int argc, char *argv[]){
	
	int i;		        		// loop variables
	int numChildren;			// how many children to create
	int idx;                    // process index
	int parent;                 // flag for fork-loop breakout
	pid_t pidTemp;              // temp pid
	
	struct message msg;			// messages to send or recv

	// check usage
	if(argc != 2){
		printf("Usage: ./S09-q7 numOfChildren (int)\n");
		exit(-1);
	}

	numChildren = atoi(argv[1]);	// get the number of children from cmd params
	int pipes[numChildren+1][2];	// pipes for all the processes
	

	// Set up the pipes - including one for the parent
	for(i = 0; i < numChildren + 1; i++){
		if(pipe(pipes[i]) == -1) {
			perror("pipe");
			exit(-1);
		}
	}

	// fork all the processes
	for(i = 0; i < numChildren; i++){
		switch(fork()){
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

		// children break out of fork-loop
		if(parent == 0){
			break;
		}
	}

	// parent reads all messages sent from children
    if(parent == 1){
    	// close all pipes for children
    	for(i = 0; i < numChildren; i++){
    		close(pipes[i][0]);
    		close(pipes[i][1]);
    	}

    	// parent closes its write end
    	close(pipes[numChildren][1]);

    	//read and print messages
        while(read(pipes[numChildren][0], &msg, sizeof(msg)) == sizeof(msg)){		
			printf("Process %d received message from Process %d\n", msg.my_pid, msg.received_pid);
			fflush(stdout);
   		}

   		// close read end and exit
   		close(pipes[numChildren][0]);
   		exit(0);
    }
    else{
		//  child closes read end of all pipes except its own
		for(i = 0; i < numChildren + 1; i++){
			if(idx != i){
				close(pipes[i][0]);
			}
		}

		// child gets its proces id from kernel
		pidTemp = getpid();
	   
	   // If process id is odd ( and not original process), child writes its messages
	   	if(pidTemp%2){
	   		for(i = 0; i < numChildren; i++){
	   			if(idx != i){
	   				if(write(pipes[i][1], &(pidTemp), sizeof(pidTemp)) != sizeof(pidTemp)){
	   					perror("write");
	   					exit(-1);
	   				}
	   			}
	   		}
	   	}
	   
	   // close write ends of pipes of all children
	   	for(i = 0; i < numChildren; i++) {
	   			close(pipes[i][1]);
	   	}

	   	// child then send recieved messages to parent
	   	msg.my_pid = pidTemp;
	   	while(read(pipes[idx][0], &(msg.received_pid), sizeof(msg.received_pid)) == sizeof(msg.received_pid)){		
			if(write(pipes[numChildren][1], &msg, sizeof(msg)) != sizeof(msg)){
				perror("child write to parent");
				exit(-1);
			}
	   	}

	   	// child closes remaining open pipes
	   	close(pipes[numChildren][1]);
	   	close(pipes[idx][0]);
		exit(0);	
	}
}
