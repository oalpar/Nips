// Copyright (c) 2011 Google, Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// CityHash, by Geoff Pike and Jyrki Alakuijala
//
// http://code.google.com/p/cityhash/
//
// This file provides a few functions for hashing strings.  All of them are
// high-quality functions in the sense that they pass standard tests such
// as Austin Appleby's SMHasher.  They are also fast.
//
// For 64-bit x86 code, on short strings, we don't know of anything faster than
// CityHash64 that is of comparable quality.  We believe our nearest competitor
// is Murmur3.  For 64-bit x86 code, CityHash64 is an excellent choice for hash
// tables and most other hashing (excluding cryptography).
//
// For 64-bit x86 code, on long strings, the picture is more complicated.
// On many recent Intel CPUs, such as Nehalem, Westmere, Sandy Bridge, etc.,
// CityHashCrc128 appears to be faster than all competitors of comparable
// quality.  CityHash128 is also good but not quite as fast.  We believe our
// nearest competitor is Bob Jenkins' Spooky.  We don't have great data for
// other 64-bit CPUs, but for long strings we know that Spooky is slightly
// faster than CityHash on some relatively recent AMD x86-64 CPUs, for example.
// Note that CityHashCrc128 is declared in citycrc.h.
//
// For 32-bit x86 code, we don't know of anything faster than CityHash32 that
// is of comparable quality.  We believe our nearest competitor is Murmur3A.
// (On 64-bit CPUs, it is typically faster to use the other CityHash variants.)
//
// Functions in the CityHash family are not suitable for cryptography.
//
// Please see CityHash's README file for more details on our performance
// measurements and so on.
//
// WARNING: This code has been only lightly tested on big-endian platforms!
// It is known to work well on little-endian platforms that have a small penalty
// for unaligned reads, such as current Intel and AMD moderate-to-high-end CPUs.
// It should work on all 32-bit and 64-bit platforms that allow unaligned reads;
// bug reports are welcome.
//
// By the way, for some hash functions, given strings a and b, the hash
// of a+b is easily derived from the hashes of a and b.  This property
// doesn't hold for any hash functions in this file.

#ifndef CITY_HASH_H_
#define CITY_HASH_H_

#include <stdlib.h>  // for size_t.
#include <stdint.h>
#include <utility>

typedef std::pair<uint64, uint64> uint128;
//not using these


__mm256i City_Hash64WithSeed(__mm256i reg, size_t len, uint64 seed) ;
__mm 256i CityHash64WithSeeds(__mm256i reg,__mm256i  mm_len, uint64 seed0, uint64 seed1);
__mm256i CityHash64(__mm256i reg, __mm256i mm_len);
__mm256i HashLen0to16(__mm256i reg, __mm256i mm_len);
__mm256i HashLen16(__m256i u64,__m256i v64, uint64 mul);
static __mm256i Rotate32(uint32 val, int shift) ;
 //Starts there

// Hash 128 input bits down to 64 bits of output.
// This is intended to be a reasonably good hash function.
inline __m256i Hash128to64(__mm256i x, __mm256i seed){
  // Murmur-inspired hashing.
 // const uint64 kMul = 0x9ddfea08eb382d69ULL;
 // uint64 a = (Uint128Low64(x) ^ Uint128High64(x)) * kMul;
 // a ^= (a >> 47);
 // uint64 b = (Uint128High64(x) ^ a) * kMul;
 // b ^= (b >> 47);
 // b *= kMul;
  uint64 arr[4];
  std::fill_n(arr,4,kMul);
  __m256i mm_kMul= _mm256_load_si256((__m256i *) arr);
  __m256i a=_mm256_xor_si256(x,seed);
  __m256i a1=mul64_haswell (a,mm_kMul);
  __m256i a11=_mm256_slli_epi64(a1,47);
   __m256i b=_mm256_xor_si256(seed,a11);
  __m256i b1=mul64_haswell (b,mm_kMul);
  __m256i b11=_mm256_slli_epi64(b1,47);
  __m256i b2_=mul64_haswell (b11,mm_kMul);
  return b2;
}

#endif  // CITY_HASH_H_
