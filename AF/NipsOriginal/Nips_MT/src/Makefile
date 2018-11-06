CPPFLAGS	= -O3 -std=c++11 -mavx -mavx2 
CC			= g++
MM			= framework/MurmurHash3.cpp
B2			= framework/blake2b-ref.c


default : all

all : testtime


testtime : timetest.cpp
	${CC} ${CPPFLAGS} timetest.cpp ${MM} ${B2} -o testtime


clean :
	rm -f testtime 
	rm -f *.o
	rm -f *.exe
