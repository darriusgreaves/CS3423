{ FEED[$5] = 1 }
NR == 1 {startTime = $1 " " $2 " " $3}
$4 == "+" {ACCEPT[$5]++}
$4 == "j" {ACCEPT[$5]++}
$4 == "-" {REJECT[$5]++}
$4 == "c" {CANCEL[$5]++; ACCEPT[$5]++}
END{
    printf "\n%45s\n\n\n", "Incoming News Feed Summary";
    printf "%-22s  %-10s    %-10s    %-10s\n", "", "accepted", "rejected", "canceled";
    for(i in FEED){
        printf "%-22s  %-10d    %-10d    %-10d\n", i, ACCEPT[i]+0, REJECT[i]+0,CANCEL[i]+0;
    }
    msg = $1 " " $2 " " $3;
    printf "\nStart Time = %-25s End Time = %s\n\n", startTime, msg;
}       
