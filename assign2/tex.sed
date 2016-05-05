#!/bin/sed -f
# sed script to convert ascii file to tex
s/\\/\\verb+\\+/g
s/%/\\%/g
s/\^/\\\^/g
/^\s*.[+-]/ s/ *$/ \\\\/
9s/ \\\\//
s/--/-\\hspace{.01cm}-/g
/^[A-Z]/ s/^/\\item[/
1s/\\item\[//
/^\\item\[/ s/$/] \\hfill \\\\/
1 i\\\documentstyle[11pt]{article}
1 i\\\begin{document}
1 i\\\begin{center} {\\bf
1 a\} \\end{center}
1 a\\\begin{description}
$ a\\\end{description}
$ a\\\end{document}
