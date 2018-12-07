CPPFLAGS	= -O3 -std=c++11 -mavx -mavx2 -I framework
CC			= g++
MM			= framework/MurmurHash3.cpp
B2			= framework/blake2b-ref.c
<<<<<<< HEAD
CTC			= framework/city.cc
=======
CTH			= framework/city.h
CSC			= framework/citysimd.cpp
CSH			= framework/citysimd.h
HM			= framework/hashing_more.h
>>>>>>> 717740e0dffc39ee8122b37930900fcc37af7c1d

default : all

all : testtime


testtime : timetest.cpp
<<<<<<< HEAD
	${CC} ${CPPFLAGS} timetest.cpp ${MM} ${B2} ${CTC} -o testtime
=======
	${CC} ${CPPFLAGS} timetest.cpp ${MM} ${B2} ${CTH} ${CSC} ${CSC} ${CSH} ${HM} -o testtime
>>>>>>> 717740e0dffc39ee8122b37930900fcc37af7c1d


clean :
	rm -f testtime 
	rm -f *.o
	rm -f *.exe
