CPPFLAGS	= -O3 -std=c++11 -mavx -mavx2 -I framework
CC			= g++
MM			= framework/MurmurHash3.cpp
B2			= framework/blake2b-ref.c
CTC			= framework/city.cc

default : all

all : test_city


test_city : test_city.cpp
	${CC} ${CPPFLAGS} test_city.cpp ${MM} ${B2} ${CTC} -o test_city


clean :
	rm -f test_city
	rm -f *.o
	rm -f *.exe
