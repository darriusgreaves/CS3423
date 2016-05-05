$7 == "group" { NGART[$6, $8] += $9 }
$7 == "exit"  { ARTICLE[$6] += $9; GROUP[$6] += $11 }
$7 == "times" { ELAPSE[$6] += $13 }
NR == 1 {startTime = $1 " " $2 " " $3}
END{
for(i in ARTICLE){
    if(match(i, /ring[0-4][0-9].cs.utsa.edu/)){
        ringArtTot += ARTICLE[i];
        ringGrpTot += GROUP[i];
        ringCs += NGART[i, "utsa.cs.2413"];
        ringCsD += NGART[i, "utsa.cs.2413.d"];
        ringTime += ELAPSE[i];
    }
}
printf "\n%50s\n\n", "News Reader Summary";
printf "\n%15s %-15s %-15s %-15s %-15s\n\n", "", "lonestar", "runner", "ringer", "rings";
printf "%-15s %-15d %-15d %-15d %-15d\n", "Articles:", ARTICLE["lonestar.jpl.utsa.edu"]+0, ARTICLE["runner.jpl.utsa.edu"]+0, ARTICLE["ringer.cs.utsa.edu"]+0, ringArtTot+0;
printf "%-15s %-15d %-15d %-15d %-15d\n", "Groups:", GROUP["lonestar.jpl.utsa.edu"]+0, GROUP["runner.jpl.utsa.edu"]+0, GROUP["ringer.cs.utsa.edu"]+0, ringGrpTot+0;
printf "%-15s %-15d %-15d %-15d %-15d\n", "Cs2413:", NGART["lonestar.jpl.utsa.edu","utsa.cs.2413"]+0, NGART["runner.jpl.utsa.edu","utsa.cs.2413"]+0, NGART["ringer.cs.utsa.edu","utsa.cs.2413"]+0, ringCs+0;
printf "%-15s %-15d %-15d %-15d %-15d\n", "Cs2413.d:", NGART["lonestar.jpl.utsa.edu","utsa.cs.2413.d"]+0, NGART["runner.jpl.utsa.edu","utsa.cs.2413.d"]+0, NGART["ringer.cs.utsa.edu","utsa.cs.2413.d"]+0, ringCsD+0;
printf "%-15s %-15.3f %-15.3f %-15.3f %-15.3f\n", "User Time:", ELAPSE["lonestar.jpl.utsa.edu"]+0, ELAPSE["runner.jpl.utsa.edu"]+0, ELAPSE["ringer.cs.utsa.edu"]+0, ringTime+0;
msg = $1 " " $2 " " $3;
printf "\nStart Time = %-25s End Time = %s\n\n", startTime, msg;
}
