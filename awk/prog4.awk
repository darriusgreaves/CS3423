BEGIN{ R0 = 0; R1 = 0; R2 = 0; R3 = 0; R4 = 0}
/Ascend-Data-Rate/ {
    if( $3 >= 0 && $3 <= 14400) R0++;
    else if( $3 <= 19200) R1++;
    else if( $3 <= 28800) R2++;
    else if( $3 <= 33600) R3++;
    else  R4++;
}
END{ 
print "0-14400		" R0+0;
print "14401-19200	" R1+0;
print "19201-28800	" R2+0;
print "28801-33600	" R3+0;
print "above 33600	" R4+0;
}
