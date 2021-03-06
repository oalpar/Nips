
#ifndef _HASHING_MORE_H_1
#define _HASHING_MORE_H_1

#include <cstdint>

// TODO: If you have a seed of random bytes you should place it in the seed
// directory and use randomgen.h instead!
#include <random> // TODO: Replace with #include "randomgen.h"

#include "MurmurHash3_og.h"
#include "MurmurHash3.h"
/* *************************************************************
 * Wrapper for MurMurHash similar to the rest
 * *************************************************************/


class murmurwrap1
{
    uint32_t m_seed;

public:
    murmurwrap1();
    void init();
    uint32_t operator()(uint32_t x);
};

murmurwrap1::murmurwrap1() { }

void murmurwrap1::init()
{
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<uint32_t> dist;
	 m_seed = dist(rng);

	
    // TODO: Replace with the line below if using randomgen.h
    //m_seed = getRandomUInt32();
}

uint32_t murmurwrap1::operator()(uint32_t x)
{
    uint32_t h;
    m_seed=12345; //comment this out if working for real
    h=0;
    MurmurHash3_x86_32(&x, 4, m_seed, &h);
	

    return h;
}





class murmurwrap
{
    uint32_t m_seed;

public:
    murmurwrap();
    void init();
    __m256i operator()(__m256i x);
};

murmurwrap::murmurwrap() { }

void murmurwrap::init()
{
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<uint32_t> dist;
    m_seed = dist(rng);
    // TODO: Replace with the line below if using randomgen.h
    //m_seed = getRandomUInt32();
}

__m256i murmurwrap::operator()(__m256i x)
{
    m_seed=12345;
    __m256i h;
    MurmurHash3_x86_323(&x, 4, m_seed, &h);
    return h;
}




#endif //_HASHING_MORE_H_
