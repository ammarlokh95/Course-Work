JC = javac

default: compile run700M

compile: 
	$(JC) hw1.java
run700M: compile
	java hw1 -NUMBINS 7 -DATASIZE 700000000 -NOOFTHREADS 1

run7M: compile
	java hw1 -NUMBINS 7 -DATASIZE 7000000 -NOOFTHREADS 1

run70k: compile
	java hw1 -NUMBINS 7 -DATASIZE 700000 -NOOFTHREADS 1

run4t700M: compile
	java hw1 -NUMBINS 7 -DATASIZE 700000000 -NOOFTHREADS 4

run2t700M: compile
	java hw1 -NUMBINS 7 -DATASIZE 700000000 -NOOFTHREADS 2

run4t70k: compile
	java hw1 -NUMBINS 7 -DATASIZE 70000 -NOOFTHREADS 4

run2t70k: compile
	java hw1 -NUMBINS 7 -DATASIZE 70000 -NOOFTHREADS 2

run4t7M: compile
	java hw1 -NUMBINS 7 -DATASIZE 7000000 -NOOFTHREADS 4

run2t7M: compile
	java hw1 -NUMBINS 7 -DATASIZE 7000000 -NOOFTHREADS 2

clean: 
	rm *.class
