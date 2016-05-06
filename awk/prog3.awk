BEGIN{ $tot = 0}
/Acct-Session-Time/ {tot += $3}
END{ print "Total User Session Time =", tot}
