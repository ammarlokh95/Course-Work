#!/usr/bin/python

import sys

fil = sys.argv[1]
f = file(fil,"r")

ch = f.readline()

for c in f :
   ch += c[:]
f.close()

c=ch.split();
d={}
ch=""
flag=0

for i in c :
	if i.isdigit():
		if flag==1:
			d[a]=ch
		else:
			 flag=1
		ch=""
		a=int(i)
	else:
		ch+= i+ " "
d[a]=ch

c= d.keys()

c.sort()
ch=""
for i in c:
	ch+= str(i)+ " " + d[i] + "\n"
print ch
	
