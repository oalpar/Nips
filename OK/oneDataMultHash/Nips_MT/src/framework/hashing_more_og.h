
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
uint32_t seeds[8];


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
    //comment this out if working for real
    uint32_t h;

  
	MurmurHash3_x86_32(&x, 4, m_seed, &h);
    
	return h;
}





class murmurwrap
{
    __m256i m_seed1;

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
   // m_seed = dist(rng);
    // TODO: Replace with the line below if using randomgen.h
    //m_seed = getRandomUInt32();
}

__m256i murmurwrap::operator()(__m256i x)
{
    m_seed1=_mm256_load_si256((__m256i *) seeds); 
    __m256i h;
    MurmurHash3_x86_323(&x, 4, m_seed1, &h);
    uint32_t xsimd[8];

   
    return h;
}




#endif //_HASHING_MORE_H_
