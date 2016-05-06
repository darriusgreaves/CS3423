#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#define TRUE		1
#define FALSE		0

#define	MSG_MAX		10
#define NUM_WRITES	9
#define NUM_PROCS	10

#define DEAD_PROC	-1



/* Globals*/
// stores the id of the process 
int my_proc_num;
//stores the pipes of every process involved
int proc_pipes[NUM_PROCS][2];  

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

int main(int argc, char *argv[])
{
    pid_t child_pid;
    pid_t my_children[NUM_PROCS]; 

    int recipient;
    char text[MSG_MAX];

    int i, ret, fd[2];

    // create the pipe for main - checking if pipeing works
    if((ret = pipe(fd)) < 0) {
    	perror("cannot create pipes");
    	exit(-1);
    }

    // initialize process id
    my_proc_num = -1;

    // initialize the pipes for the processes
    for(i = 0; i < NUM_PROCS; i++) {
    	if((ret = pipe(proc_pipes[i])) < 0) {
    		perror("error creating pipe");
    		exit(-1);
    	}
    }

    // fork other processes [1 ... NUM_PROCS]
    for(i = 0; i < NUM_PROCS; i++) {
    	// set the child process number
    	my_proc_num++;
    	printf("process%d\n", my_proc_num);
    	fflush(stdout);

    	child_pid = fork();
    	if(child_pid ==-1){
    		perror("fork");
    		exit(-1);
    	}
    	else if(!child_pid) {
    		// child here
    		int j;
    		int randArr[NUM_WRITES];
    		
    		sleep(1);

    		// close unused pipes read sides
    		for(j = 0; j < NUM_PROCS; j++) {
    			if(j != my_proc_num){
    				close(proc_pipes[j][0]);
    			}
    		}

    		// find out who I will send messages to
    		srand(1921739 * i);
        	generateRandIndxArray(i, randArr);

        	// produce some messages
        	for(j = 0; j < NUM_WRITES; j++) {
        		sleep(1);

        		// get the recipient
        		recipient = randArr[j];

        		// prepare the message
        		snprintf(text, MSG_MAX, "process%d", my_proc_num);
        		ret = write(proc_pipes[recipient][1], text, MSG_MAX);
        		
        	}

        	// now read all messages sent to me
        	while(1) {
        		char buf[MSG_MAX];
        		ret = read(proc_pipes[my_proc_num][0], &buf, MSG_MAX);
        		if(ret < 0) {
        			// bad message
        			//printf(" did not get a message\n");
        			break;
        		}
       			// good message, print it
       			printf("process%d has received a message from %s\n", my_proc_num, buf);
       			sleep(1);
       			fflush(stdout);
        	}

        	// now child can exit
        	sleep(1);
        	close(proc_pipes[my_proc_num][0]);
        	//printf("%d has exited\n", my_proc_num);
        	exit(0);
        }

        // mark the childs pid 
        my_children[my_proc_num] = child_pid;

        // close my read end
        sleep(2*my_proc_num+1);
        close(proc_pipes[my_proc_num][0]);

        
    }

    // all done?
    //printf("terminating all processes\n");
    for(i = 0; i < NUM_PROCS; i++) {
    	//printf("%d ", my_children[i]);
    	if(my_children[i] != -1){
    		kill(my_children[i], SIGTERM);
    	}
    }
    //printf("Done!\n");
    return 0;
}
