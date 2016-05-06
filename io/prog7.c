#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char * argv[]) {
    int fdin,i,n;
    unsigned int buf[1024];
    struct product {
        int type;
        int number;
    } p;
    
    if ( argc != 2 ) {
        fprintf(stderr,"Usage: prog6 <file.in>\n");
        exit(-1);
    }
    
    if((fdin = open(argv[1], O_RDONLY)) < 0 ) {
        perror(argv[1]);
        exit(-1);
    }
    
    while( (n = read(fdin, &p, sizeof(p))) == sizeof(p) ) {
        printf("Product\t%d\tQuantity\t%d\n",p.type, p.number);
        
        if ( read(fdin,buf,p.number*sizeof(int)) != p.number*sizeof(int) ) {
            fprintf(stderr,"Partial Write or Write error\n");
            exit(-1);
        }
        for(i=0; i<p.number; i++) {
            printf("\t%10u\n",buf[i]);
        }
        printf("\n");
        
    }
    
    if ( n < 0 ) {
        perror("Write error\n");
        exit(-1);
    } else if ( n > 0 ) {
        printf("Error: Incomplete Record\n");
    }
    
    close(fdin);
    exit(0);
}
