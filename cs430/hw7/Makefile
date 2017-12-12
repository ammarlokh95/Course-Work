default: compile
compile:
	g++ -std=c++11 -o hw7 hw7.cpp
run: compile
	./hw7 -f bound-cow.smf -X 0.35 -Y -0.3 -Z 0.3 -u -0.35 -v -0.35 -j 43 -k 71 -o 201 -p 402 -P
test: compile
	./hw7 –f bound-sprellpsd.smf> out1.xpm
	./hw7 -f bound-cow.smf >out2.xpm
	./hw7 -f bound-cow.smf -F 0.1 -B -0.18 >out3.xpm
	./hw7 -u -1.0 -U 0.8 -v -0.9 -V 1.0 -g bound-bunny_1k.smf -f bound-sprtrd.smf -j 34 -k 104 -o 149 -p 472 >out6.xpm
	./hw7 -f bound-bunny_1k.smf -g bound-cow.smf -i bound-sprtrd.smf >out4.xpm
	./hw7 -f bound-bunny_1k.smf -g bound-cow.smf -i bound-sprtrd.smf -q 0.4 -r 0.2 -w 1.0 -P >out5.xpm
	./hw7 -u -1.0 -U 0.8 -v -0.9 -V 1.0 -g bound-bunny_1k.smf -f bound-sprtrd.smf -j 34 -k 104 -o 149 -p 472 -Q -1.1 -R 0.9 -W 0.6 >out7.xpm
	
compilegdb:
	g++ -std=c++11 -g hw7.cpp
all: compile run 
clean: 
	rm -f *.xpm
