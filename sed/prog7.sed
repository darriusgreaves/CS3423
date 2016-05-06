#!/bin/sed -f -rn
/Invalid user/!d
s/^.*Invalid user //
s/ from//
