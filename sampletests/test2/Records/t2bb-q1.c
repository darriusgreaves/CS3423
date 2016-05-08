/*
Write a C function with prototype:
	int delrecs(char *fdb, int rnum, int num, int rsz);
This function will delete the num records, starting with record rnum in the file whose name is
given by fdb. Notice that there cannot be holes in the file, i.e. the resulting file must contain
the remaining records as contiguous records. You may assume that the file contains at least
rnum + num records.
Furthermore you may assume that the file consist of an integral number of records and that
all records have the same size, rsz. A record is merely a consecutive block of bytes of size
rsz. The record indexing starts at 0 so record 0 consists of bytes 0 through rsz − 1, record
1 consists of bytes rsz through 2rsz − 1, and so on. The function must both open and
close the file and read/writes must be record sized. If successful, delrecs() will return a 0
else delrecs() will return -1. The function must use low-level I/O except for printing error
messages and you may assume that rsz ≤ 1024.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>

int delrecs(char *fdb, int rnum, int num, int rsz){
	int i;				// loop variables
	int fd;				// file discriptor
	char *buf[rsz];		// buffer to keep record in
	int fsize;			// size of file

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

	// loop over record after rnum + num 
	for(i = rnum+num; i < fsize/rsz -1; i++){

		// seek to first record to keep
		if(lseek(fd, i*rsz, SEEK_SET) < 0){
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

		// seek up to the place to write
		if(lseek(fd, (i - num)*rsz, SEEK_SET) < 0){
			close(fd);
			perror("seek");
			return -1;
		}

		// write the record
		if(write(fd, buf, rsz) != rsz){
			close(fd);
			perror("write");
			return -1;
		}
	}

	// truncate the file
	ftruncate(fd, fsize - num*rsz);

	// close the file and return
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
	int rnum;		// delete records starting at rnum rec
	int num;		// number of record to delete
	int ret;		// return value of method

	//check usage
	if(argc != 4){
		printf("Usage: t2aa file rnum num\n");
		exit(-1);
	}

	// set file and number from cmd params
	file = argv[1];
	rnum = atoi(argv[2]);
	num = atoi(argv[3]);

	// create a new record
	rec = get_timestamp();
	rsz = strlen(rec);

	// delete record
	delrecs(file, rnum, num, rsz);

	// print message to user and exit
	printf("delrecs returned: %d\n", ret);
	exit(0);
}