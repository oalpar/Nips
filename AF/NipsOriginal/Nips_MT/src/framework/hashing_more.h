/* ***************************************
 * Wrappers for crypto hash funcitons
 * ***************************************/

#ifndef _HASHING_MORE_H_
#define _HASHING_MORE_H_

#include <cstdint>

// TODO: If you have a seed of random bytes you should place it in the seed
// directory and use randomgen.h instead!
#include <random> // TODO: Replace with #include "randomgen.h"
#include <immintrin.h>
#include "MurmurHash3.h"
#include "blake2.h"
#include "blake2-impl.h"
#include "city.h"

/* *************************************************************
 * Wrapper for MurMurHash similar to the rest
 * *************************************************************/

class murmurwrap
{
    uint32_t m_seed;

public:
    murmurwrap();
    void init();
    uint32_t operator()(uint32_t x);
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

uint32_t murmurwrap::operator()(uint32_t x)
{
    uint32_t h;
    MurmurHash3_x86_32(&x, 4, m_seed, &h);
    return h;
}

/* **************************************************************
 * Blake2 wrapper
 * **************************************************************/

class blake2wrap
{
    uint32_t m_seed;
public:
    blake2wrap();
    void init();
    uint32_t operator()(uint32_t x);
};

blake2wrap::blake2wrap() { }

void blake2wrap::init()
{
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<uint32_t> dist;
    m_seed = dist(rng);
    // TODO: Replace with the line below if using randomgen.h
    //m_seed = getRandomUInt32();
}

uint32_t blake2wrap::operator()(uint32_t x)
{
    uint32_t h;
    blake2b(&h, 4, &x, 4, NULL, 0);
    return h;
}

/* **************************************************************
 * CityHash wrapper
 * **************************************************************/

class citywrap
{
    uint64_t m_seed;
public:
    citywrap();
    void init();
    uint32_t operator()(uint32_t x1);
};

citywrap::citywrap() { }

void citywrap::init()
{
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<uint64_t> dist;
    m_seed = dist(rng);
    // TODO: Replace with the line below if using randomgen.h
    //m_seed = getRandomUInt64();
}

uint32_t citywrap::operator()(uint32_t x1)
{
    uint32_t h;
    h = (uint32_t)CityHash64WithSeed((const char *)&x1, 4, m_seed);
    return h;
}

class s_citywrap
{
    uint64_t m_seed;
	__m256i mm_seed1;
	
public:
    s_citywrap();
    void init(int);
    __m256i operator ()(__m256i x);
};

s_citywrap::s_citywrap() { }

void s_citywrap::init(int m)
{
	
	std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<uint64_t> dist;
	uint64 arr[4];
    if (m==1){
	m_seed = dist(rng);
	 std::fill_n(arr,4,m_seed);
	mm_seed1= _mm256_load_si256((__m256i *) arr);
	}
	else
	{
	for(int i=0; i<4;i++)
		{
		m_seed = dist(rng);
		arr[i]=m_seed;
		}
	mm_seed1= _mm256_load_si256((__m256i *) arr);
	}
}

__m256i s_citywrap::operator () (__m256i x)
{
    __m256i h;
    h = CitysimdHash64WithSeed(x, 4, mm_seed1);
    return h;
}

#endif 
//_HASHING_MORE_H_
