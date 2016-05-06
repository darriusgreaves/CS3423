

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include "logprt.h"

int main (int argc, char * argv[]) {
    int fdin;    

    /* Check usage */
    if ( argc != 2 ) {
        fprintf(stderr,"Usage: logprt <file>\n");
        exit(-1);
     }
 
    /* Open file or die */
    if((fdin = open(argv[1], O_RDONLY)) < 0 ) {
        perror(argv[1]);
        exit(-1);
    }
  
    /* Read and print file header information */
    fileHeaderIO(fdin);
    
    /* Read and print the packets */
    packetIO(fdin);
    
    /* Close and exit */
    close(fdin);
    exit(0);
}


/* Reads and prints the file header */
void fileHeaderIO(int fdin) {
    int n = read(fdin, &h, sizeof(h));
    if (n != sizeof(h)) {
        close(fdin);
        perror("Error Reading pcap file header");
        exit(-1);
    }

    printFileType(h.magic);
    printf("Version major number = %d\n", h.version_major);
    printf("Version minor number = %d\n", h.version_minor);
    printf("GMT to local correction = %d\n", h.thiszone);
    printf("Timestamp accuracy = %d\n", h.sigfigs);
    printf("Snaplen = %u\n", h.snaplen);
    printf("Linktype = %d\n\n", h.linktype);  
}

/* Prints the type of file */
void printFileType(u_int32_t magic) {
    if (magic == PCAP_MAGIC)
        printf("PCAP_MAGIC\n");
    else if(magic == PCAP_SWAPPED_MAGIC)
        printf("PCAP_SWAPPED_MAGIC\n");
    else if(magic == PCAP_MODIFIED_MAGIC)
        printf("PCAP_MODIFIED_MAGIC\n");
    else if(magic == PCAP_SWAPPED_MODIFIED_MAGIC)
        printf("PCAP_SWAPPED_MODIFIED_MAGIC\n");
    else
        printf("UNKNOWN FILE TYPE");
}

/* Reads and prints the packet information */
void packetIO(int fdin) {
    int n;
    while( n = read(fdin, &p, sizeof(p)) == sizeof(p) ) {
        printPacketHeader();
        
        /* skipping over info in packet */
        lseek(fdin,p.len, SEEK_CUR);
        printf("\n");
    }

    if ( n == -1 ) {
        close(fdin);
        perror("Write error\n");
        exit(-1);
    } else if ( n > 0 ) {
        printf("Error: Incomplete Record\n");
    }
}


/* Prints the packet header information*/
void printPacketHeader(void) {
    static int count = 0;
    printf("Packet %d\n", count);
    printTime();
    printf("Captured Packet Length = %d\n", p.caplen);
    printf("Actual Packet Length = %d\n", p.len);
    count++;
}

/* Prints the packet time */
void printTime(void) {
    static int firsttime = 1;
    static unsigned int b_sec;
    static int b_usec;
    
    unsigned int c_sec;
    int c_usec;
    
    if(firsttime) {
        firsttime = 0;
        b_sec = p.ts.tv_sec;
        b_usec = p.ts.tv_usec;
    }
    
    c_sec = (unsigned)p.ts.tv_sec - b_sec;
    c_usec = (unsigned)p.ts.tv_usec - b_usec;
    
    while(c_usec < 0) {
        c_usec += 1000000;
        c_sec--;
    }
    
    printf("%05u.%06u\n", (unsigned)c_sec, (unsigned)c_usec);
}
