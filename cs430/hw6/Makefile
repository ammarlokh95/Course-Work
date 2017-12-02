default: compile
compile:
	g++ -std=c++11 -o hw6 hw6.cpp
run: compile
	./hw6 -f bound-cow.smf -X 0.35 -Y -0.3 -Z 0.3 -u -0.35 -v -0.35 -j 43 -k 71 -o 201 -p 402 -P


test: compile
	./hw6 -f bound-lo-sphere.smf > out1.xpm
	./hw6 -f bound-lo-sphere.smf -P > out2.xpm
	./hw6 -f bound-lo-sphere.smf -q 1.0 >out3.xpm
	./hw6 -f bound-lo-sphere.smf -x -4.0 -z 5.0 -q 1.0 -w -0.5 >out4.xpm
	./hw6 -f bound-lo-sphere.smf -k 125 -p 375 -q 1.0 -u -1.4 -U 1.4 >out5.xpm
	./hw6 -f bound-cow.smf -X 0.35 -Y -0.3 -Z 0.3 -u -0.35 -v -0.35 -j 43 -k 71 -o 201 -p 402 -P >out6.xpm
	./hw6 -f bound-lo-sphere.smf -x -1.0 -z 0.5 -q 1.0 -w -0.5 >out7.xpm
	./hw6 -f bound-bunny_200.smf -j 100 -k 50 -o 400 -p 450 -x 0.5 -y 0.2 -z 1.0 -X 0.2 -Y -0.2 -Z 0.3 -q -3.0 -r -2.0 -w 1.0 -Q 3.0 -R -2.0 -W -4.0 -u -0.5 -U 1.2 -V 0.8 -P >out8.xpm
	./hw6 -f bound-cow.smf >out9.xpm
	./hw6 -f bound-cow.smf -P >out10.xpm
	./hw6 -f bound-cow.smf -j 0 -k 30 -o 275 -p 305 -P >out11.xpm
	./hw6 -f bound-cow.smf -x 1.5 >out12.xpm
	./hw6 -f bound-cow.smf -x 4.75 -y -3.25 -z 3.3 -P >out13.xpm
	./hw6 -f bound-cow.smf -X 0.25 -Y -0.15 -Z 0.3 >out14.xpm
	./hw6 -f bound-cow.smf -X 0.35 -Y -0.3 -Z 0.3 -u -0.35 -v -0.35 -P >out15.xpm
	./hw6 -f bound-cow.smf -X 0.25 -Y -0.15 -Z 0.3 -j 103 -k 143 -o 421 -p 379 >out16.xpm
	./hw6 -f bound-cow.smf -X 0.35 -Y -0.3 -Z 0.3 -u -0.35 -v -0.35 -j 43 -k 71 -o 201 -p 402 -P >out17.xpm
	./hw6 -f bound-cow.smf -q -1 -r 1.5 -w -2.0 >out18.xpm
	./hw6 -f bound-cow.smf -Q 1.5 -R 1 -W 0.4 >out19.xpm
	./hw6 -f bound-cow.smf -u -1.5 -v -0.9 -U 1.2 -V 0.7 >out20.xpm
test_ds: compile
	./hw6 -f bound-lo-sphere.smf -ds > out1_ds.xpm
	./hw6 -f bound-lo-sphere.smf -P -ds > out2_ds.xpm
	./hw6 -f bound-lo-sphere.smf -q 1.0 -ds >out3_ds.xpm
	./hw6 -f bound-lo-sphere.smf -x -4.0 -z 5.0 -q 1.0 -w -0.5 -ds >out4_ds.xpm
	./hw6 -f bound-lo-sphere.smf -k 125 -p 375 -q 1.0 -u -1.4 -U 1.4 -ds >out5_ds.xpm
	./hw6 -f bound-cow.smf -X 0.35 -Y -0.3 -Z 0.3 -u -0.35 -v -0.35 -j 43 -k 71 -o 201 -p 402 -P -ds >out6_ds.xpm
	./hw6 -f bound-lo-sphere.smf -x -1.0 -z 0.5 -q 1.0 -w -0.5 -ds >out7_ds.xpm
	./hw6 -f bound-bunny_200.smf -j 100 -k 50 -o 400 -p 450 -x 0.5 -y 0.2 -z 1.0 -X 0.2 -Y -0.2 -Z 0.3 -q -3.0 -r -2.0 -w 1.0 -Q 3.0 -R -2.0 -W -4.0 -u -0.5 -U 1.2 -V 0.8 -P -ds >out8_ds.xpm
	./hw6 -f bound-cow.smf -ds >out9_ds.xpm
	./hw6 -f bound-cow.smf -P -ds >out10_ds.xpm
	./hw6 -f bound-cow.smf -j 0 -k 30 -o 275 -p 305 -P -ds >out11_ds.xpm
	./hw6 -f bound-cow.smf -x 1.5 -ds >out12_ds.xpm
	./hw6 -f bound-cow.smf -x 4.75 -y -3.25 -z 3.3 -P -ds >out13_ds.xpm
	./hw6 -f bound-cow.smf -X 0.25 -Y -0.15 -Z 0.3 -ds >out14_ds.xpm
	./hw6 -f bound-cow.smf -X 0.35 -Y -0.3 -Z 0.3 -u -0.35 -v -0.35 -P -ds >out15_ds.xpm
	./hw6 -f bound-cow.smf -X 0.25 -Y -0.15 -Z 0.3 -j 103 -k 143 -o 421 -p 379 -ds >out16_ds.xpm
	./hw6 -f bound-cow.smf -X 0.35 -Y -0.3 -Z 0.3 -u -0.35 -v -0.35 -j 43 -k 71 -o 201 -p 402 -P -ds >out17_ds.xpm
	./hw6 -f bound-cow.smf -q -1 -r 1.5 -w -2.0 -ds >out18_ds.xpm
	./hw6 -f bound-cow.smf -Q 1.5 -R 1 -W 0.4 -ds >out19_ds.xpm
	./hw6 -f bound-cow.smf -u -1.5 -v -0.9 -U 1.2 -V 0.7 -ds >out20_ds.xpm

compilegdb:
	g++ -std=c++11 -g hw5.cpp
all: compile run 
clean: 
	rm -f *.xpm
