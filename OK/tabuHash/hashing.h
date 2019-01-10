/* ***********************************************
 * Hash functions:
 * This file define several hash functions as
 *
 * classes.
 * If DEBUG is defined it asserts that initialization
 * is done properly.
 * ***********************************************/

//#ifndef _HASHING_H_
//#define _HASHING_H_
#include <cassert>
#include <vector>
#include <cstdint>
#include <immintrin.h>
#ifdef DEBUG
#include <cassert>
#endif
#include <random>
#include <iostream>
// TODO: Replace with: #include "randomgen.h"
// TODO: If you have a seed of random bytes (from e.g. random.org) you can use
// the randomgen file instead to provide random numbers.
using namespace std;


/* ***********************************************
 * Multiply-shift hashing a la Dietzfelbinger
 * ***********************************************/

/*
  class multishift
{
#ifdef DEBUG
  bool hasInit;
#endif
uint32_t m_a, m_b;
  
 public:
  multishift();
  uint32_t operator()(uint32_t x);
  void init(uint32_t x1, uint32_t y1);
};
multishift::multishift()
{

#ifdef DEBUG
  hasInit=false;
#endif
}

void multishift::init(uint32_t x1, uint32_t y1)
{
  m_a=x1;
  m_b=y1;
  // TODO: Replace with the lines below if using randomgen.h
  //m_a = getRandomUInt64();
  //m_b = getRandomUInt64();
  #ifdef DEBUG
  hasInit=true;
  #endif
}

uint32_t multishift::operator()(uint32_t x)
{
  uint32_t ret;
  ret=m_a*x + m_b;
  return ret;
}
class s_multishift
{
#ifdef DEBUG
    bool hasInit;
#endif
    
    __m256i m_ma, m_mb;
 public:
    s_multishift();
    __m256i operator()(__m256i th_8);
    void init(int m);
};

s_multishift::s_multishift()
{
#ifdef DEBUG
  hasInit=false;
#endif
}

void s_multishift::init(int m)
{
	std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<uint64_t> dist;
	uint64 arr[8];
	uint64 arr1[8];
	
  if(m==1){
  int ma_seed = dist(rng);
	std::fill_n(arr,8,m_seed);
	m_ma= _mm256_load_si256((__m256i *) arr);
  int mb_seed = dist(rng);
	std::fill_n(arr1,8,m_seed);
	m_mb= _mm256_load_si256((__m256i *) arr1);
#ifdef DEBUG
    hasInit=true;
#endif
  }
  else
  {
	for(int i=0; i<8;i++)
		{
		int ma_seed = dist(rng);
		arr[i]=ma_seed;
		}
	m_ma= _mm256_load_si256((__m256i *) arr);
	}
	for(int i=0; i<8;i++)
		{
		int mb_seed = dist(rng);
		arr[i]=mb_seed;
		}
	m_mb= _mm256_load_si256((__m256i *) arr);
	}
  }
}
__m256i s_multishift::operator()(__m256i th_8)
{
#ifdef DEBUG
  assert(hasInit);
#endif
  __m256i mxx=_mm256_mullo_epi32(th_8, m_ma);
  __m256i myy=_mm256_add_epi32(mxx,m_mb);
  return myy;
}


#endif // _HASHING_H_
/*
/* ***************************************************
 * Poly hashing with large degree a la Carter & Wegman
 * ***************************************************/

class polyhash
{
//#ifdef DEBUG
    bool hasInit;
//#endif
    uint32_t m_deg;
    std::vector<uint64_t> m_coef;
    // Large mersenne prime (2^61 - 1)
    const uint64_t m_p = 2305843009213693951;

public:
    polyhash();
    void init(); // 2-indep
    void init(uint32_t deg);
    uint32_t operator()(uint32_t x);
};

polyhash::polyhash()
{
//#ifdef DEBUG
    hasInit=false;
//#endif
}

void polyhash::init()
{
    init(2);
}

void polyhash::init(uint32_t deg)
{
    // TODO: Remove the following 3 lines if using randomgen.h
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<uint64_t> dist;

    m_deg = deg;
    m_coef.resize(m_deg,0);
    for (uint32_t i = 0; i < m_deg; ++i) {
	m_coef[i]=i;
	/*	do {
            // TODO: Swap the two lines below if using randomgen.h
			//m_coef[i] = getRandomUInt64() >> 3;
			m_coef[i] = (dist(rng)) >> 3;
//#ifdef DEBUG
			assert(m_coef[i] <= m_p); //Since poly_p = 2^61-1.
//#endif
		} while(m_coef[i] >= m_p);*/
    }
//#ifdef DEBUG
    hasInit=true;
//#endif
}

uint32_t polyhash::operator()(uint32_t x)
{
//#ifdef DEBUG
    assert(hasInit);
//#endif
    __int128 h = 0;
    for (int32_t i = m_deg-1; i >= 0; --i) {
		h = h * x + m_coef[i];
		h = (h & m_p) + (h >> 61);
    }
	h = (h & m_p) + (h >> 61);
    return (uint32_t)h;
}

/* ***************************************************
 * Simple Tabulation
 * ***************************************************/

class simpletab
{
    uint32_t m_T[256][4];

public:
    simpletab();
    void init();
    uint32_t operator()(uint32_t x);
};

simpletab::simpletab()
{
}

void simpletab::init()
{
    // Use a degree-20 polynomial to fill out the entries.
    polyhash h;
    h.init(20);

        uint32_t x = 0;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 256; ++j)
            m_T[j][i] = h(x++);
}

uint32_t simpletab::operator()(uint32_t x)
{
    uint32_t h=0; // Final hash value
    for (int i = 0; i < 4; ++i, x >>= 8)
        h ^= m_T[(uint8_t)x][i];
    return h;
}



/* ***************************************************
 * Simple Tabulation SIMD
 * ***************************************************/

class s_simpletab
{
    uint32_t m_T[256][4];
	__m256i mm_T[256][4];
	int choice;

public:
    s_simpletab();
    void init(int ch);
    __m256i operator()(uint32_t x,__m256i y, int ch);
};

s_simpletab::s_simpletab()
{
}

void s_simpletab::init(int ch)
{
    // Use a degree-20 polynomial to fill out the entries.
    polyhash h;
    h.init(20);
	__m256i temp;
	choice=ch;
	uint32_t arr[8];
	if( ch==1)
	{
    uint32_t x = 0;
    for (int i = 0; i < 4; ++i)
	{
        for (int j = 0; j < 256; ++j)
		{ m_T[j][i] = h(x++);}

	}
	}
	else
	{
	    uint32_t x = 0;
   	 for (int i = 0; i < 4; ++i)
      {
	  for (int j = 0; j < 256; ++j)
        {
			for(int k=0; k<8; k++)
			{	arr[k]=h(x++);
			}
			temp= _mm256_load_si256((__m256i *) arr);
			mm_T[j][i]=temp;
		}	//m_T[j][i] = h(x++);
	}
	}
}

__m256i s_simpletab::operator()(uint32_t x,__m256i x1,int ch)
{
	if(ch==1)
	{
	uint32_t arr_h[8];
	uint32_t arr_x[8];
	uint32_t h=0; // Final hash value
	std::fill_n(arr_h,8,h);
	__m256i temp;
	__m256i m_h= _mm256_load_si256((__m256i *) arr_h);
	for (int i = 0; i < 4; ++i)
	{
		for(int k=0; k<8;k++)
		{
		arr_x[k]=_mm256_extract_epi32(x1,k);
		arr_h[k]=m_T[(uint8_t)arr_x[k]][i];
		}
		temp= _mm256_load_si256((__m256i *) arr_h);
		 m_h= _mm256_xor_si256(m_h,temp);
		x1=	_mm256_srli_epi32 (x1, 8);
	}
	return m_h;
	}
	else
	{
	uint32_t arr[8];
    	uint32_t h=0; // Final hash value
	std::fill_n(arr,8,h);
	__m256i m_h= _mm256_load_si256((__m256i *) arr);
    //for (int i = 0; i < 4; ++i, x >>= 8)
    //{ 
	int i=0;
    __m256i tempo= _mm256_xor_si256(m_h,mm_T[(uint8_t)x][i]);//h ^= m_T[(uint8_t)x][i];// m_h= mh op m_T[][] might be a problem due to simd object
     i=i+1;
	 x >>= 8;
	 __m256i tempo1= _mm256_xor_si256(tempo,mm_T[(uint8_t)x][i]);
	 i=i+1;
	 x >>= 8;
	 __m256i tempo2= _mm256_xor_si256(tempo1,mm_T[(uint8_t)x][i]);
	 i=i+1;
	 x >>= 8;
	 __m256i tempo3= _mm256_xor_si256(tempo2,mm_T[(uint8_t)x][i]);
	//}
	return tempo3;
	}
}
