#!/bin/sed -f
/user=root/!d
s/.*logname=//
s/ uid=.*//
