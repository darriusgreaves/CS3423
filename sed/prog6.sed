#!/bin/sed -f
/verification/!d
s/.*warning: //
s/: hostname /\t/
s/ verification.*//
