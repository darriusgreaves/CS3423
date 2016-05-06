#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int fill_buffer(int size);
void get_particle(void);
float read_float(void);
void print_particle(void);
struct vector {
	float x;
	float y;
	float z;
};

struct particle {
	float mass;
	struct vector pos;
	struct vector vel;
} p;

struct buffer {
  int fd;
  int size;
  char *pos;
  char *end;
  char buf[1024];
} b;

int main (int argc, char * argv[])
  {
  
     if ( argc != 2 ) {
       fprintf(stderr,"Usage: prog4 <file>\n");
       exit(-1);
     }
  
    if((b.fd = open(argv[1], O_RDONLY)) < 0 ) {
      perror(argv[1]);
      exit(-1);
    }
    struct stat st;
    fstat(b.fd, &st);
    int fsize = st.st_size;
    
    int psize = (int) sizeof(p);
    
    while(fsize > 0) {
        int n = fill_buffer(psize);
        if (n < psize) {
        	perror("Error Reading pcap file header");
        	exit(-1);
    	}
        get_particle();
        print_particle();
        fsize -= n;
    }
    
    close(b.fd);
    exit(0);
}

/* Fills buffer structure with data from the file */
int fill_buffer(int size) {
    int n;

    if ( (n = read(b.fd,b.buf,size)) == -1 ) {
        perror("Read error");
        exit(-1);
    }

    b.size = n;
    b.pos = b.buf;
    b.end = b.buf + n;
    
    return(n);
}

void get_particle(void) {
	p.mass  = read_float();
        p.pos.x = read_float();
        p.pos.y = read_float();
        p.pos.z = read_float();
        p.vel.x = read_float();
        p.vel.y = read_float();
        p.vel.z = read_float();
}

void print_particle(void) {
	static int count = 1;
	printf("Particle\t%d\n", count);
	printf("\tmass\t%f\n",p.mass);
	printf("\tpos\t(%f,%f,%f)\n", p.pos.x, p.pos.y, p.pos.z);
	printf("\tvel\t(%f,%f,%f)\n", p.vel.x, p.vel.y, p.vel.z);
	count++;
}

float read_float(void) {
    char temp[4];
    temp[0] = *b.pos++;
    temp[1] = *b.pos++;
    temp[2] = *b.pos++;
    temp[3] = *b.pos++;
    return *(float *) temp;
}

