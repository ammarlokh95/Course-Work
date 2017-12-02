#!/bin/bash
# to count the number lines and words for each file in a directory

for f in * 
	do
		wc -m -l $f
done 
