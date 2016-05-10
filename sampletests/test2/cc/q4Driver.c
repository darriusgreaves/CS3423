/*
	Driver for t2cc-q4
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void){
	char *prog;
	char *argv[4];
	int fdread;
	int fdwrite;
	int fd[2];
	char c = 'A';
	pipe(fd);

	fdread = fd[0];
	fdwrite = fd[1];
	printf("Before: fdread %d, fdwrite %d\n", fdread, fdwrite);
	prog = "./t2cc-q1";
	argv[0] = "rec1.txt";
	argv[1] = "12";
	argv[2] = "rec2.txt";
	argv[3] = "2";

	pd2open(prog, argv, &fdread, &fdwrite);

	printf("After: fdread %d, fdwrite %d\n", fdread, fdwrite);
	while(read(fdread, &c, 1) == 1){
		write(fdwrite, &c, 1);
	}
}