default: compile run1

compile:
	g++ -std=c++11 -o hw5 hw5.cpp
run1: compile
	./hw5 -f hw5_1.ps > hw5_1.xpm
run_nofill: compile
	./hw5 -f hw5_1.ps -fp n > hw5_1_nofill.xpm
runtests: compile
	./runtests.sh
runtests_nofill: compile
	./runtests_nofill.sh
compilegdb:
	g++ -std=c++11 -g hw5.cpp
all: compile run1 
clean: 
	rm -f *.xpm
