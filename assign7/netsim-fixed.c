#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define TRUE		1
#define FALSE		0

#define	MSG_MAX		10
#define NUM_WRITES	9
#define NUM_PROCS	10

/* Create the rand index array*/
void generateRandIndxArray(int indx, int *array) {
    int i, j, rng, num;

    rng = RAND_MAX/NUM_PROCS;
    rng *= NUM_PROCS;
    for (i = 0; i < NUM_WRITES; i++) {
        do {
            num = rand();
            j = num % NUM_PROCS;
        }
        while (num >= rng || indx == j);
        array[i] = j;
    }
}


int main(void){
	
	int i;		        		// loop variables
	int pipes[NUM_PROCS][2];	// pipes for all the processes
	int randArr[NUM_WRITES];	// array to hold random indices
	int recipient;				// who we will write to
	int idx;                    // process index
	int parent;                 // parent of fork

	// message struct used to send and recieve messages
	struct message {
		int index;
		char buf[MSG_MAX];
	} msg;


	// Set up the pipes
	for(i = 0; i < NUM_PROCS; i++){
		if(pipe(pipes[i]) == -1) {
			perror("pipe");
			exit(-1);
		}
	}

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

		// child breaks out of fork loop
		if(parent == 0){
			break;
		}
	}

	// parent exits, not waiting for childern
    if(parent == 1){
        exit(0);
    }
    
	//  child closes read end of all pipes except its own
	for(i = 0; i < NUM_PROCS; i++){
		if(idx != i){
			close(pipes[i][0]);
		}
	}
   
	// generate random pipe indexes to write to
	srand(1921739 * idx);
    generateRandIndxArray(i, randArr);

   // write the messages
    for(i = 0; i < NUM_WRITES; i++){
    	// get the recipient
        recipient = randArr[i];
        strcpy(msg.buf,"process");
        msg.index = idx;
        if(write(pipes[recipient][1], &msg, sizeof(msg)) != sizeof(msg)){
			perror("write");
			exit(-1);
		}
    }

    // close the write ends of all pipes
    for (i = 0; i < NUM_PROCS; ++i)
    {
    	close(pipes[i][1]);
    }

   	// child then Stays up all night reading 
   	while(read(pipes[idx][0], &msg, sizeof(msg)) == sizeof(msg)){		
		printf("process%d has received a message from %s%d\n", idx, msg.buf, msg.index);
		fflush(stdout);
   	}

   	// child closes its read end and exits
   	close(pipes[idx][0]);
	exit(0);	
}
