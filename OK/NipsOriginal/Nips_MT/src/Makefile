CPPFLAGS	= -O3 -std=c++11 -mavx -mavx2 -I/home/halpar/Nips/Nips/NipsOriginal/Nips_MT/src/framework
CC			= g++
MM			= framework/MurmurHash3.cpp
B2			= framework/blake2b-ref.c framework/city.cc


default : all

all : testtime


testtime : timetest.cpp
	${CC} ${CPPFLAGS} timetest.cpp ${MM} ${B2} -o testtime


clean :
	rm -f testtime 
	rm -f *.o
	rm -f *.exe

