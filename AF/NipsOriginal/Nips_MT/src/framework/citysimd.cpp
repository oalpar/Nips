#include "config.h"
#include "city.h"

#include <algorithm>
#include <string.h>

using namespace std;
//AGNER FOG'S VECTOR LIBRARY CODE
__m256i mul64_haswell (__m256i a, __m256i b) {
    // instruction does not exist. Split into 32-bit multiplies
    __m256i bswap   = _mm256_shuffle_epi32(b,0xB1);           // swap H<->L
    __m256i prodlh  = _mm256_mullo_epi32(a,bswap);            // 32 bit L*H products

    // or use pshufb instead of psrlq to reduce port0 pressure on Haswell
    __m256i prodlh2 = _mm256_srli_epi64(prodlh, 32);          // 0  , a0Hb0L,          0, a1Hb1L
    __m256i prodlh3 = _mm256_add_epi32(prodlh2, prodlh);      // xxx, a0Lb0H+a0Hb0L, xxx, a1Lb1H+a1Hb1L
    __m256i prodlh4 = _mm256_and_si256(prodlh3, _mm256_set1_epi64x(0x00000000FFFFFFFF)); // zero high halves

    __m256i prodll  = _mm256_mul_epu32(a,b);                  // a0Lb0L,a1Lb1L, 64 bit unsigned products
    __m256i prod    = _mm256_add_epi64(prodll,prodlh4);       // a0Lb0L+(a0Lb0H+a0Hb0L)<<32, a1Lb1L+(a1Lb1H+a1Hb1L)<<32
    return  prod;
}

#define bswap_32(x) _byteswap_ulong(x)
#define bswap_64(x) _byteswap_uint64(x)

#elif defined(__APPLE__)

// Mac OS X / Darwin features
#include <libkern/OSByteOrder.h>
#define bswap_32(x) OSSwapInt32(x)
#define bswap_64(x) OSSwapInt64(x)

#elif defined(__sun) || defined(sun)

#include <sys/byteorder.h>
#define bswap_32(x) BSWAP_32(x)
#define bswap_64(x) BSWAP_64(x)

#elif defined(__FreeBSD__)

#include <sys/endian.h>
#define bswap_32(x) bswap32(x)
#define bswap_64(x) bswap64(x)

#elif defined(__OpenBSD__)

#include <sys/types.h>
#define bswap_32(x) swap32(x)
#define bswap_64(x) swap64(x)

#elif defined(__NetBSD__)

#include <sys/types.h>
#include <machine/bswap.h>
#if defined(__BSWAP_RENAME) && !defined(__bswap_32)
#define bswap_32(x) bswap32(x)
#define bswap_64(x) bswap64(x)
#endif

#else

#include <byteswap.h>

#endif

#ifdef WORDS_BIGENDIAN
#define uint32_in_expected_order(x) (bswap_32(x))
#define uint64_in_expected_order(x) (bswap_64(x))
#else
#define uint32_in_expected_order(x) (x)
#define uint64_in_expected_order(x) (x)
#endif

#if !defined(LIKELY)
#if HAVE_BUILTIN_EXPECT
#define LIKELY(x) (__builtin_expect(!!(x), 1))
#else
#define LIKELY(x) (x)
#endif
#endif

static uint64 ShiftMix(uint64 val) { // <3
  return val ^ (val >> 47);
}

static uint64 Fetch64(const char *p) {
  return uint64_in_expected_order(UNALIGNED_LOAD64(p));
}

static uint32 Fetch32(const char *p) {
  return uint32_in_expected_order(UNALIGNED_LOAD32(p));
}

// Some primes between 2^63 and 2^64 for various uses.
static const uint64 k0 = 0xc3a5c85c97cb3127ULL;
static const uint64 k1 = 0xb492b66fbe98f273ULL;
static const uint64 k2 = 0x9ae16a3b2f90404fULL;

// Magic numbers for 32-bit hashing.  Copied from Murmur3.
static const uint32 c1 = 0xcc9e2d51;
static const uint32 c2 = 0x1b873593;

// A 32-bit to 32-bit integer hash copied from Murmur3.
static __mm256i fmix(uint32 h)
{
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;
  return h;
}

static __mm256i Rotate32(uint32 val, int shift) {
  // Avoid shifting by 32: doing so yields an undefined result.
  return shift == 0 ? val : ((val >> shift) | (val << (32 - shift)));
}


__mm256i Fetch64(const char *p) {
  return uint64_in_expected_order(UNALIGNED_LOAD64(p));
}

__mm256i Fetch32(const char *p) {
  return uint32_in_expected_order(UNALIGNED_LOAD32(p));
}

__mm256i HashLen16(uint64 u, uint64 v) {
  return Hash128to64(uint128(u, v));
}


__mm256i HashLen16(__m256i u64,__m256i v64, uint64 mul) {
  // Murmur-inspired hashing.
  //uint64 a = (u ^ v) * mul;
  //a ^= (a >> 47);
  //uint64 b = (v ^ a) * mul;
  //b ^= (b >> 47);
  //b *= mul;
  //MULTIPLY U64 AND V64 BEFORE PASSING TO THIS FUNCTION???? there is no 64bit multiplication in simd
  uint64 arr[4];
  std::fill_n(arr,4,mul);
  __m256i mm_mul= _mm256_load_si256((__m256i *) arr);
  __m256i a=_mm256_xor_si256(u64,v64);
  __m256i a1=mul64_haswell (a,mm_mul);
  __m256i a11=_mm256_slli_epi64(a1,47);
  __m256i b=_mm256_xor_si256(v64,a11);
  __m256i b1=mul64_haswell (b,mm_mul);
  __m256i b11=_mm256_slli_epi64(b1,47);
  __m256i b2_=mul64_haswell (b11,mm_mul);
  return b2;
}

__mm256i HashLen0to16(__mm256i reg, __mm256i mm_len) {
  if (len >= 8) {
    uint64 mul = k2 + len * 2;
    uint64 a = Fetch64(s) + k2;
    uint64 b = Fetch64(s + len - 8);
    uint64 c = Rotate(b, 37) * mul + a;
    uint64 d = (Rotate(a, 25) + b) * mul;
    return HashLen16(c, d, mul);
  }
  if (len >= 4) {
    uint64 mul = k2 + len * 2;
    //uint64 a = Fetch32(s);
    uint64 arr[4];
    std::fill_n(arr_a,4,4);
    __mm256i a4= _mm256_load_si256((__m256i *) arr);
    __mm256i t=_mm256_slli_epi64(reg,3);
    __mm256i t1=_mm256_add_epi64(t, mm_len);
    __mm256i len_4=_mm256_sub_epi64 (Ãmm_len, a4)
    // return HashLen16(len+ (a<<3), Fetch32(s + len - 4), mul);
    return HashLen16(t1,len_4,mul)
  }
  if (len > 0) {
    uint8 a = s[0];
    uint8 b = s[len >> 1];
    uint8 c = s[len - 1];
    uint32 y = static_cast<uint32>(a) + (static_cast<uint32>(b) << 8);
    uint32 z = len + (static_cast<uint32>(c) << 2);
    return ShiftMix(y * k2 ^ z * k0) * k2;
  }
  return k2;
}


__mm256i CityHash64(__mm256i reg, __mm256i mm_len) {
  //if (len <= 32) {
  //if (len <= 16) {
      return HashLen0to16(reg, mm_len);
      //} else {
      //return HashLen17to32(s, len);
      //}
      //} else if (len <= 64) {
      //return HashLen33to64(s, len);
  }


__mm 256i CityHash64WithSeeds(__mm256i reg,__mm256i  mm_len,
                           uint64 seed0, uint64 seed1) {
  uint64 arr[4],arr1[4];
  std::fill_n(arr,4,seed1);
  __mm256i mm_seed1= _mm256_load_si256((__m256i *) arr);
  std::fill_n(arr1,4,seed0);
  __mm256i mm_seed0= _mm256_load_si256((__m256i *) arr1);
  __mm256ix =CityHash64(reg,mm_len)
  __mm256i x1=_mm256_sub_epi64 (x,mm_seed0)

 return HashLen16(x, mm_seed1);
}

__mm256i CityHash64WithSeed(__mm256i reg, size_t len, uint64 seed) { //Starts there
  uint64 lenth=len; 
  uint64 arr[4];
  std::fill_n(arr,4,length);
   __mm256i mm_len= _mm256_load_si256((__m256i *) arr);
 
  return CityHash64WithSeeds(reg, mm_len, k2, seed);
}



  
  



  
  
