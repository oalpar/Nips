#include "config.h"
#include "city.h"

#include <algorithm>
#include <string.h>

using namespace std;


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
static uint32 fmix(uint32 h)
{
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;
  return h;
}

static uint32 Rotate32(uint32 val, int shift) {
  // Avoid shifting by 32: doing so yields an undefined result.
  return shift == 0 ? val : ((val >> shift) | (val << (32 - shift)));
}


static uint64 Fetch64(const char *p) {
  return uint64_in_expected_order(UNALIGNED_LOAD64(p));
}

static uint32 Fetch32(const char *p) {
  return uint32_in_expected_order(UNALIGNED_LOAD32(p));
}

static uint64 HashLen16(uint64 u, uint64 v) {
  return Hash128to64(uint128(u, v));
}


static uint64 HashLen16(uint64 u, uint64 v, uint64 mul) {
  // Murmur-inspired hashing.
  uint64 a = (u ^ v) * mul;
  a ^= (a >> 47);
  uint64 b = (v ^ a) * mul;
  b ^= (b >> 47);
  b *= mul;
  return b;
}

static uint64 HashLen0to16(const char *s, size_t len) {
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
    uint64 a = Fetch32(s);
    return HashLen16(len + (a << 3), Fetch32(s + len - 4), mul);
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


uint64 CityHash64(const char *s, size_t len) {
  //if (len <= 32) {
  //if (len <= 16) {
      return HashLen0to16(s, len);
      //} else {
      //return HashLen17to32(s, len);
      //}
      //} else if (len <= 64) {
      //return HashLen33to64(s, len);
  }


uint64 CityHash64WithSeeds(const char *s, size_t len,
                           uint64 seed0, uint64 seed1) {
  return HashLen16(CityHash64(s, len) - seed0, seed1);
}

uint64 CityHash64WithSeed(const char *s, size_t len, uint64 seed) { //Starts there
  return CityHash64WithSeeds(s, len, k2, seed);
}



  
  



  
  
