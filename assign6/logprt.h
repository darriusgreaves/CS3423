

/* Define constants */
#define BUFSZ                           1024
#define PCAP_MAGIC                      0xa1b2c3d4
#define PCAP_SWAPPED_MAGIC              0xd4c3b2a1
#define PCAP_MODIFIED_MAGIC             0xa1b2cd34
#define PCAP_SWAPPED_MODIFIED_MAGIC     0x34cdb2a1

/* Function Prototypes */
void fileHeaderIO(int fdin);
void printFileType(u_int32_t magic);

void packetIO(int fdin);
void printPacketHeader(void);
void printTime(void);

/* pcap data stored once at the beginning of the file */
struct pcap_file_header {
    u_int32_t magic;
    u_int16_t version_major;
    u_int16_t version_minor;
    u_int32_t thiszone;
    u_int32_t sigfigs;
    u_int32_t snaplen;
    u_int32_t linktype;
} h;

/* System time info for packed*/
struct timev {
    unsigned int tv_sec;
    unsigned int tv_usec;
};

/* data prefixing each packet */
struct my_pkthdr {
    struct timev ts;
    int caplen;
    int len;
} p;
