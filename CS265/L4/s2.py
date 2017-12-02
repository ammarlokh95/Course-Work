#!/usr/bin/python

import sys

fil = sys.argv[1];

f = file(fil,"r")

ch = f.readline()

c = ch[:];

for c in f :
	ch += c[:]
f.close()

c= ch.replace(',',' ').split()

ch=""
flag = 1
s=0
n=0
for i in c:
	if i.isalpha():
		if flag==1:
			ch += i
			flag=0
		else:
			ch += " " + str(s/n) + "\n" + i
		s=0
		n=0
	else:
		s+=int(i)
		n+=1
ch+= " " + str(s/n)		
print ch
