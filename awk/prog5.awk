BEGIN{ 
  IBUTot = 0; 
  OBUTot = 0;
}
/Acct-Input-Packets/{ IBUTot += $3 }
/Acct-Output-Packets/{ OBUTot += $3 }
END{
  print "Total Input Bandwidth Usage  =", IBUTot, "packets";
  print "Total Output Bandwidth Usage =", OBUTot, "packets";
}
