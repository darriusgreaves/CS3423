#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char * argv[]) {
  int fdin,i,n;

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

  if ( argc != 2 ) {
    fprintf(stderr,"Usage: prog4 <file.in>\n");
    exit(-1);
  }

  if((fdin = open(argv[1], O_RDONLY)) < 0 ) {
    perror(argv[1]);
    exit(-1);
  }

  i = 1;
  while ( (n = read(fdin,&p,sizeof(p))) == sizeof(p) ) {
    printf("Particle	%d\n",i++);
    printf("\tmass\t%f\n",p.mass);
    printf("\tpos\t(%f,%f,%f)\n",p.pos.x,p.pos.y,p.pos.z);
    printf("\tvel\t(%f,%f,%f)\n",p.vel.x,p.vel.y,p.vel.z);
  }

  if ( n == -1 ) {
    perror("Write error\n");
    exit(-1);
  } else if ( n > 0 ) {
    printf("Error: Incomplete Record\n");
  }
  close(fdin);
  exit(0);
}
