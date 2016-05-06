/* prtlog.c
    Autor:  Crissy Ruffo (vud447)
    Date:   March 2016
    Purpose:    Partial fulfillment of assignment 6.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include "pcap.h"
#include "dnet.h"
#include "prtlog.h"


int main (int argc, char * argv[]) {
    int fdin;

    if ( argc != 2 ) {
        fprintf(stderr,"Usage: logprt <file>\n");
        exit(-1);
     }
  
    if((fdin = open(argv[1], O_RDONLY)) < 0 ) {
        perror(argv[1]);
        exit(-1);
    }

    /* Read and print file header information */
    fileHeaderIO(fdin);

    /* Read and print the packets */
    packetIO(fdin);
    
    close(fdin);
    exit(0);
}

/* Reads and prints the file header */
void fileHeaderIO(int fdin) {
    struct pcap_file_header h;
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
        int m;
        printPacketHeader();
        m = ethHeaderIO(fdin); 
        /* skipping over rest of packet */
        lseek(fdin,p.len - m, SEEK_CUR);
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


int ethHeaderIO(int fdin) {
    struct eth_hdr eh; 
    int n = read(fdin, &eh, sizeof(eh)); 
    if(n != sizeof(eh)) {
        close(fdin);
        perror("Error Reading packet");
        exit(-1);
    }

    printf("Ethernet Header\n");
    switch(ntohs(eh.eth_type)) {
        case ETH_TYPE_IP:
            n += ipHeaderIO(fdin);
            break;
        case ETH_TYPE_ARP:
            n += arpHeaderIO(fdin);
            break;
       case ETH_TYPE_PUP:
            printf("\tPUP\n");
            break;
       case ETH_TYPE_REVARP:
            printf("\tREVARP\n");
            break;
       case ETH_TYPE_8021Q:
            printf("\t8021Q\n");
            break;
       case ETH_TYPE_IPV6:
            printf("\tIPV6\n");
            break;
       case ETH_TYPE_MPLS:
            printf("\tMPLS\n");
            break;
       case ETH_TYPE_MPLS_MCAST:
            printf("\tMPLS_MCAST\n");
            break;
       case ETH_TYPE_PPPOEDISC:
            printf("\tPPPOEDISC\n");
            break;
       case ETH_TYPE_PPPOE:
            printf("\tPPPOE\n");
            break;
       case ETH_TYPE_LOOPBACK:
            printf("\tLOOPBACK\n");
            break;
       default:
            printf("\tUNRECOGNIZED\n");
    }
    return n;
}


/* Reads the arp header information */
int arpHeaderIO(int fdin) {
    struct arp_hdr ah;
    int n = read(fdin, &ah, sizeof(ah)); 
    if(n != sizeof(ah)) {
        close(fdin);
        perror("Error Reading packet");
        exit(-1);
    }
    printArpOp(ah.ar_op);
    return n;
}

/* Prints the arp operation type */
void printArpOp(uint16_t arpOp) {
    printf("\tARP\n");
    switch(ntohs(arpOp)) {
        case ARP_OP_REQUEST:
            printf("\t\tarp operation = Arp Request\n");
            break;
        case ARP_OP_REPLY:
            printf("\t\tarp operation = Arp Reply\n");
            break;
        case ARP_OP_REVREQUEST:
            printf("\t\tarp operation = Arp Reverse Request\n");
            break;
        case ARP_OP_REVREPLY:
            printf("\t\tarp operation = Arp Reverse Reply\n");
            break;
        default:
            printf("\t\tarp operation = UNRECOGNIZED\n");
            break;
    }
}

/* Reads the ip header information */
int ipHeaderIO(int fdin) {
    struct ip_hdr iph;
    int n = read(fdin, &iph, sizeof(iph)); 
    if(n != sizeof(iph)) {
        close(fdin);
        perror("Error reading ip header");
        exit(-1);
    }
    printIpProtocal(iph.ip_p);
    return n;
}


/* Prints the ip protocol type */
void printIpProtocal(uint8_t protocol) {
    printf("\tIP\n");
    switch(protocol) {
        case IP_PROTO_ICMP:
            printf("\t\tICMP\n");
            break;
        case IP_PROTO_IGMP:
            printf("\t\tIGMP\n");
            break;
        case IP_PROTO_TCP:
            printf("\t\tTCP\n");
            break;
        case IP_PROTO_UDP:
            printf("\t\tUDP\n");
            break;
        default:
            printf("\t\tUNRECOGNIZED\n");
            break;
    }
}

