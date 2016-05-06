#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char * argv[]) {
    int fdin,i,n;

    #include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main (int argc, char * argv[]) {
    int fdin,i,n;
    char buf[1024];
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
        
        if ( read(fd,buf,p.quan*sizeof(int)) != p.quan*sizeof(int) ) {
            fprintf(stderr,"Partial Write or Write error\n");
            exit(-1);
        }
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
        
        if ( read(fd,buf,p.quan*sizeof(int)) != p.quan*sizeof(int) ) {
            fprintf(stderr,"Partial Write or Write error\n");
            exit(-1);
        }
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
