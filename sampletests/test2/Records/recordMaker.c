// Makes a record file to be used with problems 
// dealing with adding, deleting, moving, etc...
// records

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>

#define RCSZ	24	// size of a timestamp

// gets a time stamp as a record
char * get_timestamp(void){
	time_t now = time(NULL);
	return asctime(localtime(&now));
}

int main(int argc, char *argv[]){
	int i;			// loop variable
	int fd;			// file discriptor
	int numRecs;	// number of record to write
	char *file;		// name of file to create

	// check usage
	if(argc != 3){
		printf("Usage: rcmkr[create file] [num of recs] (will create or truncate file)\n");
		exit(-1);
	}
	file = argv[1];
	numRecs = atoi(argv[2]);

	// create or truncate the record file
	if((fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) {
		perror("open");
		exit(-1);
	}

	// write message to user
	printf("Writing %d records to %s\n", numRecs, argv[1]);

	// create and write records
	for(i = 0; i < numRecs; i++){
		// create record
		char *timestamp = get_timestamp();

		// write record
		if((write(fd, timestamp, RCSZ) != RCSZ) || (write(fd, "\n", 1) != 1)){
			perror("write");
			exit(-1);
		}

		// message to user
		printf(". ");
		fflush(stdout);

		// since records are timestamps, wait a bit so 
		// record aren't all the same
		sleep(2);
		
	}

	//write message to user and exit
	close(fd);
	printf("done\n Size of each record:\t 25\n");
	exit(0);
}