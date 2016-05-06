#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
int funct1(int n) {
	pid_t pid;
	int i;
        int my_children[n];
	for(i = 0; i < n; i++) {
		switch(pid = fork()) {
			case -1:
                                perror("fork");
                                exit(-1);
                        case 0:
                                printf("%d\t%d\n", i+1, getpid());
                                sleep(10);
                                exit(1);
                        default:
                                my_children[i] = pid;
				sleep(4);
		}
	}
	// all done?
	//printf("terminating all processes\n");
	for(i = 0; i < n; i++) {
		//printf("%d ", my_children[i]);
		if(my_children[i] != -1){
			kill(my_children[i], SIGTERM);
		}
	}
	//printf("Done!\n");

	return 0;
}

int main(int argc, char * argv[]) {
	if(argc != 2){
		printf("usage: funct1 (int value)\n");
		exit(-1);
	}
	int pass;
	if((pass = atoi(argv[1])) == 0) {
		printf("value must be an integer and greater than  0\n");
		exit(-1);
	}
	int ret = funct1(pass);
	//printf("funct1 returned %d\n", ret);
	sleep(5);

	return 0;
}
