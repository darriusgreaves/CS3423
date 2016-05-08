/*
Write a C function with prototype:
int insrec(char *fdb1, int rnum, char *fdb2, int num, int rsz);
This function will read the record num from the file fdb2 and insert it into record position
rnum in file fdb1. No record should be overwritten, the new record is simply inserted into
that position leaving the order of all other records, before and after, the same.
Furthermore you may assume that the files consist of an integral number of records and that
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


int insrec(char *fdb1, int wnum, char *fdb2, int rnum, int rsz) {
	int i;			// loop variable
	int fd1;		// file descriptor for fdb1
	int fd2;		// file descriptor for fdb2
	int fsize;		// size of fdb1
	char buf[rsz];	// buffer for record

	// open the files
	if((fd1 = open(fdb1, O_RDWR)) < 0){
		perror("open");
		return -1;
	}
	if((fd2 = open(fdb2, O_RDONLY)) < 0){
		perror("open");
		return -1;
	}

	// get fdb1 size
	if((fsize = lseek(fd1, 0, SEEK_END)) < 0){
		close(fd1);
		perror("seek");
		return -1;
	}

	// create hole in fdb1
	// start at bottom of file and work up
	for(i = fsize/rsz - 1; i > wnum; i--){
		// seek to record to be moved
		if(lseek(fd1, (i-1) * rsz, SEEK_SET) < 0){
			close(fd1);
			perror("seek");
			return -1;
		}
		//read the record
		if(read(fd1, buf, rsz) != rsz){
			close(fd1);
			perror("read");
			return -1;
		}
		//write the record in the next (lower) position
		if(write(fd1, buf, rsz) != rsz){
			close(fd1);
			perror("write");
			return -1;
		}
	}

	// read record from fdb2
	// seek to record to be moved
	if(lseek(fd2, rnum * rsz, SEEK_SET) < 0){
		close(fd2);
		perror("seek");
		return -1;
	}
	// read the record
	if(read(fd2, buf, rsz) != rsz){
		close(fd2);
		perror("read");
		return -1;
	}

	// write record from fdb2 to fdb1
	// seek to the hole created
	if(lseek(fd1, wnum * rsz, SEEK_SET) < 0){
		close(fd1);
		perror("seek");
		return -1;
	}
	//write the record
	if(write(fd1, buf, rsz) != rsz){
		close(fd1);
		perror("write");
		return -1;
	}

	// close both files and return
	close(fd1);
	close(fd2);
	return 0;
}



// gets a time stamp as a record
char * get_timestamp(void){
	time_t now = time(NULL);
	return asctime(localtime(&now));
}

int main(int argc, char *argv[]){
	char *rec;		// record
	char *fdb1;		// file to write record to
	char *fdb2;		// file to read record from
	int rsz;		// size of records
	int wnum;		// record number to write in fdb1
	int rnum;		// record number to read in fdb2
	int ret;		// return value of method

	//check usage
	if(argc != 5){
		printf("Usage: t2cc file1 wnum file2 rnum\n");
		exit(-1);
	}

	// set file and number from cmd params
	fdb1 = argv[1];
	wnum = atoi(argv[2]);
	fdb2 = argv[3];
	rnum = atoi(argv[4]);

	// create a new record
	rec = get_timestamp();
	rsz = strlen(rec);

	// delete record
	ret = insrec(fdb1, wnum, fdb2, rnum, rsz);

	// print message to user and exit
	printf("insrec returned: %d\n", ret);
	exit(0);
}