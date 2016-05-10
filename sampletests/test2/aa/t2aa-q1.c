/* 
Write a C function with prototype:
	int insrec(char *fdb, int num, char *rec, int rsz);
This function will insert the record stored at memory location rec AFTER record number
num in the file whose name is given by fdb. Notice that a hole in the file must be made in
order to insert the new record in its current position. You may assume that the file contains
at least num records.
Furthermore you may assume that the file consist of an integral number of records and that
all records have the same size, rsz. A record is merely a consecutive block of bytes of size
rsz. The record indexing starts at 0 so record 0 consists of bytes 0 through rsz − 1, record 1
consists of bytes rsz through 2rsz − 1, and so on. The function must both open and close the
file and read/writes must be record sized. If successful, insrec() will return a 0 else insrec()
will return -1. The function must use low-level I/O except for printing error messages and
you may assume that rsz ≤ 1024.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>


// inserts the record
int insrec(char *fdb, int num, char *rec, int rsz) {
	int i;			// loop variable
	int fd;			// file descriptor
	int fsize;		// file size
	char buf[rsz];	// buffer to hold a record

	// open the file
	if((fd = open(fdb, O_RDWR)) < 0){
		perror("open");
		return -1;
	}

	// get file size
	if((fsize = lseek(fd, 0, SEEK_END)) < 0){
		close(fd);
		perror("seek");
		return -1;
	}

	// create hole
	// start at bottom of file and work up
	for(i = fsize/rsz - 1; i > num; i--){
		// seek to record to be moved
		if(lseek(fd, (i-1) * rsz, SEEK_SET) < 0){
			close(fd);
			perror("seek");
			return -1;
		}
		//read the record
		if(read(fd, buf, rsz) != rsz){
			close(fd);
			perror("read");
			return -1;
		}
		//write the record in the next (lower) position
		if(write(fd, buf, rsz) != rsz){
			close(fd);
			perror("write");
			return -1;
		}
	}

	// seek to hole created
	if(lseek(fd, (num+1)*rsz, SEEK_SET) < 0){
		close(fd);
		perror("seek");
		return -1;
	}

	// write the given record
	if(write(fd, rec, rsz) != rsz){
		close(fd);
		perror("write");
		return -1;
	}

	// close file and return
	close(fd);
	return 0;
}


// gets a time stamp as a record
char * get_timestamp(void){
	time_t now = time(NULL);
	return asctime(localtime(&now));
}

int main(int argc, char *argv[]){
	char *rec;		// record to send
	char *file;		// file to use
	int rsz;		// size of records
	int num;		// insert new rec after num rec
	int ret;		// return value of method

	//check usage
	if(argc != 3){
		printf("Usage: t2aa file num\n");
		exit(-1);
	}

	// set file and number from cmd params
	file = argv[1];
	num = atoi(argv[2]);

	// create a new record
	rec = get_timestamp();
	rsz = strlen(rec);

	// insert record
	ret = insrec(file, num, rec, rsz);

	// print message to user and exit
	printf("insrec returned: %d\n", ret);
	exit(0);
}


