#!/bin/sed -f
# prints out a list of disk partitions followed by FS type
# fields are seperated by tabs
/^.dev.dsk/!d
s/\t.*\t[u]/\tu/
s/s\t.*/s/
s/\t[-].*\t[s]/\ts/
s/p\t.*/p/
