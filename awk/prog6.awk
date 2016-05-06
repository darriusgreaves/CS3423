BEGIN{ RS = "Mon Sep 15" }
{
if($3 == "User-Name"){ 
    user = $5; 
    if($14 == "Stop"){
        UserSes[user]++;
        UserTime[user] += $26;
        UserInP[user] += $35;
        UserOutP[user] += $38;
    }
    }else{
        user = ""; 
        UserSes[user]++;
    }
}
END{
for(i in UserSes){
    print "User:\t\t\t" i;
    print "Number of Sessions:\t" UserSes[i];
    print "Total Connect Time:\t" UserTime[i];
    print "Input Bandwidth Usage:\t" UserInP[i];
    print "Output Bandwidth Usage:\t" UserOutP[i];
    print "\n";
}
}
