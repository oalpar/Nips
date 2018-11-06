CPPFLAGS	= -O3 -std=c++11 -mavx -mavx2 -I framework
CC			= g++
MM			= framework/MurmurHash3_og.cpp framework/MurmurHash3.cpp
B2			= framework/blake2b-ref.c framework/city.cc


default : all

all : testtime


testtime : timetest.cpp
	${CC} ${CPPFLAGS} accuracyTest.cpp  ${MM} ${B2} -o murmurAcc


clean :
	rm -f testtime

	rm -f *.o
	rm -f *.exe

