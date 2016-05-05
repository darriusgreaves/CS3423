BEGIN{FS= " [0-9]+ [0-9]+ "}
{print "addgroup", $1, $2}
