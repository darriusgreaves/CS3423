#!/bin/sed -f
# changes all lines containing c1t1 from ufs to ext2
# and changes c1t1 to c1t2
/c1t1/ s/ufs/ext2/
s/c1t1/c1t2/g
