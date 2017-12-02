default: compile run1_1

compile:
	g++ -std=c++11 -o hw2 hw2.cpp
run2_1: compile
	./hw2 -f hw2_1.ps
run2_2: compile
	./hw2 -f hw2_2.ps
run2_3: compile
	./hw2 -f hw2_3.ps
run2_5: compile
	./hw2 -f hw2_5.ps
run2_4: compile
	./hw2 -f hw2_4.ps
compilegdb:
	g++ -std=c++11 -g hw2.cpp
all: compile run2_1 run2_2 run2_3 run2_4 run2_5 
