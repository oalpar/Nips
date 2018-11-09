#include <vector>
#include <cstdint>

#ifdef DEBUG
#include <cassert>
#endif

// TODO: If you have a seed of random bytes (from e.g. random.org) you can use
// the randomgen file instead to provide random numbers.
#include <random>  // TODO: Replace with: #include "randomgen.h"

/* ***********************************************                                                                                                                                                          
 * Multiply-shift hashing a la Dietzfelbinger                                                                                                                                                               
 * ***********************************************/
class org_multishift
{
#ifdef DEBUG
  bool hasInit;
#endif
  uint32_t m_a, m_b;
  
 public:
  org_multishift();
  uint32_t operator()(uint32_t x);
  void init();
};
org_multishift::org_multishift()
{
#ifdef DEBUG
  hasInit=false;
#endif
}

void org_multishift::init()
{
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<uint32_t> dist;
  m_a = dist(rng);
  m_b = dist(rng);
  // TODO: Replace with the lines below if using randomgen.h
  //m_a = getRandomUInt64();
  //m_b = getRandomUInt64();
  #ifdef DEBUG
  hasInit=true;
  #endif
}

//uint32_t org_multishift::operator()(uint32_t x)
//{
//  #ifdef DEBUG
//  assert(hasInit);
//  #endif
//  return (m_a * (uint64_t)x + m_b) >> 32;
//}
//return m_a*x + m_b

uint32_t org_multishift::operator()(uint32_t x)
{
	uint32_t ret;
	ret=m_a*x + m_b;
  return ret;
}
