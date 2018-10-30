/* ***************************************
 * Wrappers for crypto hash funcitons
 * ***************************************/

#ifndef _HASHING_MORE_H_
#define _HASHING_MORE_H_

#include <cstdint>

// TODO: If you have a seed of random bytes you should place it in the seed
// directory and use randomgen.h instead!
//#include <random> // TODO: Replace with #include "randomgen.h"
#include "randomgen.h"
#include "MurmurHash3.h"
#include "blake2.h"
#include "blake2-impl.h"

#include "city.h"

/* *************************************************************
 * Wrapper for MurMurHash similar to the rest
 * *************************************************************/

class murmurwrap //a wrapper class makes the user have an easier interface to use and can be used for garbage cleaning too
{
    uint32_t m_seed; //seed for the murmur function

public:
    murmurwrap(); //constructor
    void init(); 
    uint32_t operator()(uint32_t x);
};

murmurwrap::murmurwrap() { } //constructur doesn't do anything

void murmurwrap::init() // init initilizes the wrapper with a random number. "std::mt19937" is a random number generator introduced with c++ 11
{
   // std::mt19937 rng; //this gives MANY random numbers
  //  rng.seed(std::random_device()()); //(First () is to construct random_device(a class for generating random integer suitable for seed. object, second is for the return seed for mt19937. (ali)I failed to understand why the seed function has two ()()'s and what the seed does. When I looked up it said something about using chrono? But i think its to seed the random number generation.
//    std::uniform_int_distribution<uint32_t> dist; 
    //m_seed = dist(rng);// after all the random number generation m_seed is set to be used as the seed for our murmur hash
    // TODO: Replace with the line below if using randomgen.h
    m_seed = getRandomUInt32();
}

uint32_t murmurwrap::operator()(uint32_t x) //this is what actually matters in the wrapper. This calls the actual MurmurHash with necessary parameters
{
    uint32_t h; //murmurhash is void function and returns the output by a parameter, thats why we need this 
    MurmurHash3_x86_32(&x, 4, m_seed, &h); //&x parameter is the address of the word we will hash,  
//the second parameter is named "len" and the value we are sending is "4" because the len parameter required the length of the data to hash in bytes. so since we have intergers its 4 bytes. If we were working with MurmurHash 128 it would be 16
//the seed parameter is for us to randomize the hash. Thats why we are sending the m_seed variable we randomized earlier. m_seed is good because we should provide the same seed each time.
//The last parameter &h is our outcome. So the final hashed version.
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
    //std::mt19937 rng;
    //rng.seed(std::random_device()());
    //std::uniform_int_distribution<uint32_t> dist;
   // m_seed = dist(rng); //read murmurhash m_seed
    // TODO: Replace with the line below if using randomgen.h
    m_seed = getRandomUInt32();
}

uint32_t blake2wrap::operator()(uint32_t x)
{
    uint32_t h;
    blake2b(&h, 4, &x, 4, NULL, 0); //the first parameter &h is where our output will go  but I don't understand the reason for it because in the files blake2b is an integer returning function, so why not just use that?

//int blake2( void *out, size_t outlen, const void *in, size_t inlen, const void *key, size_t keylen ); This is the function its calling, I think outlen is the size of the thing we are outputting to, *in is the word we will hash, inlen is the size in bytes. But I dont know what key and keylen are but i dont think it matters since they are NUll and 0
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
    uint32_t operator()(uint32_t x);
};

citywrap::citywrap() { }

void citywrap::init()
{
   // std::mt19937 rng;
   // rng.seed(std::random_device()());
   // std::uniform_int_distribution<uint64_t> dist;
    //m_seed = dist(rng); //look at murmurhash m_seed
    // TODO: Replace with the line below if using randomgen.h
    m_seed = getRandomUInt64();
}

uint32_t citywrap::operator()(uint32_t x)
{
    uint32_t h;
    h = (uint32_t)CityHash64WithSeed((const char *)&x, 4, m_seed); //&x is what we hash, but city hash accepts char so we cast it. 4 is again the size in bytes but i dont know what happens to the size of x when you cast it to a char
    return h;
}

#endif //_HASHING_MORE_H_
