CPPFLAGS = -O3 -std=c++11 -mavx -mavx2 -I framework
CC = g++

default : all
all : test_tabu

testtabu : ${CC} ${CPPFLAGS} test_tabu.cpp -o tabuAcc

clean : 
	rm -f test_tabu

	rm -f *.o
	rm -f *.exe


