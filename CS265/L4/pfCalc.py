#!/usr/bin/python
#Solves postfix expressions
#inouts a postfix expression
#outputs the soultion or an error if input is incorrect

import sys
stack=[]
def is_number(s):
    try:
        float(s)
        return True
    except ValueError:
        return False

def addition():
	global stack
	x=stack.pop()
	y=stack.pop()
	stack.append(float(x)+float(y))

def subtraction(flag):
   global stack
   x=float(stack.pop())
   y=float(stack.pop())
   stack.append(y-x)

def multiplication() :
   global stack
   x=float(stack.pop())
   y=float(stack.pop())
   stack.append(x*y)

def division() :
   global stack
   x=float(stack.pop())
   y=float(stack.pop())
   stack.append(y/x)

def exponent() :
   global stack
   x=float(stack.pop())
   y=float(stack.pop())
   stack.append(y**x)

def main():
	f=sys.stdin
	flag = 0
	c=f.read( 1 )
	global stack
	ch="" 
		
	while	c :
		ch+=c
		c=f.read(1)
	l1=ch.split("\n")
	for j in l1:
		l=j.split(" ")
		for i in l :
			if is_number(i) :
				stack.append(i);
				flag+=1	
			elif i=='+' :
				if flag > 1 :
					addition()
				else:
					flag=-1
					break;
				flag-=1;
			elif i== '-' :
				if flag > 1 :
					subtraction(flag)	
				else:
					flag=-1
					break
				flag-=1
			elif i== '*' :
				if flag > 1:
					multiplication()
				else :
					flag=-1
					break
				flag-=1
			elif i == "/" :
				if flag > 1:
					division()
				else:
					flag=-1
					break
				flag-=1
			elif i == '^' :
				if flag > 1:
					exponent()
				else:
					flag=-1
					break
				flag-=1
			else :
				flag=-1;
				break;
	
		if flag != -1 :
			if len(stack) == 1 :
				print "\n"+str(stack.pop())
			else :
				print "invalid expression!"
		else:
			print "invalid Input!"


if __name__ == "__main__":
    main()
