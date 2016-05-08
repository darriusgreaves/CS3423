/*
 Write a C function with prototype:
 int delrec(char *fdb, int num, int rsz);
 This function will delete the record num from the file fdb. Since this is a sequential file of
 records the records after record num must be moved up to fill in the missing record and the
 file shortened.
 Furthermore you may assume that the file consist of an integral number of records and that
 all records have the same size, rsz. A record is merely a consecutive block of bytes of size
 rsz. The record indexing starts at 0 so record 0 consists of bytes 0 through rsz − 1, record 1
 consists of bytes rsz through 2rsz − 1, and so on. The function must both open and close the
 file and read/writes must be record sized. If successful, delrec() will return a 0 else delrec()
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

int delrec(char *fdb, int num, int rsz){
	int i;
	int fd;
	int fsize;
	char buf[rsz];

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

	// move records after num up
	for(i = num + 1; i < fsize/rsz - 1; i++){

		// seek to record to keep
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
		if(lseek(fd, (i - 1)*rsz, SEEK_SET) < 0){
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
	ftruncate(fd, fsize - rsz);

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
	int rnum;		// delete record number
	int ret;		// return value of method

	//check usage
	if(argc != 3){
		printf("Usage: t2aa file rnum\n");
		exit(-1);
	}

	// set file and number from cmd params
	file = argv[1];
	rnum = atoi(argv[2]);

	// create a new record
	rec = get_timestamp();
	rsz = strlen(rec);

	// delete record
	delrec(file, rnum, rsz);

	// print message to user and exit
	printf("delrec returned: %d\n", ret);
	exit(0);
}