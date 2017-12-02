#!/bin/bash

ls -F $1 | grep -v / > $1/temp3
find $1 -maxdepth 1 -name "[^.]*" -type d -printf '%f\n' | awk ' {print $1}' > $1/temp2

printf "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>
<direntry>\n"
if [ -a "$1/README" ]
   then
      (awk -F":" '{for (i=2; i<=NF; i++) printf "%s\n", $i}' README) > $1/temp
   (awk -F":" '{ printf "\t<%s>\n", $1; for( i=2 ; i<=NF ; i++) printf "\t\t<file>%s</file>\n", $i; printf "\t</%s>\n", $1}' < $1/README)
   diff -u $1/temp $1/temp3 > $1/temp4
fi
printf "\t<other>\n"
awk ' { printf "\t\t<dir>%s</dir>\n", $1} ' < $1/temp2
if [ -a "$1/README" ]; then
	awk ' { printf "\t\t<file>%s<file>\n", $1 }' < $1/temp4
	else
      awk ' { printf "\t\t<file>%s<file>\n", $1 }' < $1/temp3
fi
printf "\t</other>" 
printf "\n</direntry>\n</xml>" 

if [ -a $1/temp ]; then
rm -f $1/temp
fi
rm -f $1/temp2
rm -f $1/temp3 
rm -f $1/temp4
