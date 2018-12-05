CPPFLAGS	= -O3 -std=c++11 -mavx -mavx2 
CC			= g++
MM			= framework/MurmurHash3.cpp
B2			= framework/blake2b-ref.c
CTH			= framework/city.h
CSC			= framework/citysimd.cpp
CSH			= framework/citysimd.h
HM			= framework/hashing_more.h

default : all

all : testtime


testtime : timetest.cpp
	${CC} ${CPPFLAGS} timetest.cpp ${MM} ${B2} ${CTH} ${CSC} ${CSC} ${CSH} ${HM} -o testtime


clean :
	rm -f testtime 
	rm -f *.o
	rm -f *.exe
