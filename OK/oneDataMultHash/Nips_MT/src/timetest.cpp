#include "framework/hashing.h"
#include "framework/hashing_more.h"
#include <immintrin.h>
#include <iostream>
#include <ctime>
#include <vector>
#include <chrono>

#include <random>

using namespace std;


void testTime(uint32_t trials)
{
     volatile __m256i x1;
    volatile uint32_t x;  
  mt19937 rng;
    rng.seed(random_device()());
    uniform_int_distribution<uint32_t> dist;
    vector<uint32_t> nums;
    for (uint32_t i = 0; i < trials; ++i)
        nums.push_back(dist(rng));

    multishift hms;
    hms.init();
    clock_t start = clock();
    for (uint32_t i = 0; i < trials; i+=1)
        x=hms(nums[i]);
    clock_t end = clock();
    cout << "Multiply-shift & " << (float)(end-start)/CLOCKS_PER_SEC << "s \\\\" << endl;
    
    mixedtab hmt;
    hmt.init();
    start = clock();
    for (uint32_t i = 0; i < trials; ++i)
        x=hmt(nums[i]);
    end = clock();
    cout << "Mixed tabulation & " << (float)(end-start)/CLOCKS_PER_SEC << "s \\\\" << endl;
    

    polyhash2 hp2;
    hp2.init();
    start = clock();
    for (uint32_t i = 0; i < trials; ++i)
        x=hp2(nums[i]);
    end = clock();
    cout << "2-wise PolyHash & " << (float)(end-start)/CLOCKS_PER_SEC << "s \\\\" << endl;
    

    polyhash3 hp3;
    hp3.init();
    start = clock();
    for (uint32_t i = 0; i < trials; ++i)
        x=hp3(nums[i]);
    end = clock();
    cout << "3-wise PolyHash & " << (float)(end-start)/CLOCKS_PER_SEC << "s \\\\" << endl;

    polyhash hp20;
    hp20.init(4);
    start = clock();
    for (uint32_t i = 0; i < trials; ++i)
        x=hp20(nums[i]);
    end = clock();
    cout << "20-wise PolyHash & " << (float)(end-start)/CLOCKS_PER_SEC << "s \\\\" << endl;

    murmurwrap mm;
    mm.init();
    start = clock();
    
    
    trials=trials-trials%8;
    for (uint32_t i = 0; i < trials; i+=8){
	__m256i murmur;
	uint32_t arr2[8]={nums[i], nums[i+1], nums[i+2],
	nums[i+3],nums[i+4],nums[i+5],nums[i+6],nums[i+7]} ;	

	murmur=_mm256_load_si256((__m256i *)arr2);
	x1= mm(murmur);
    }
    end = clock();
    cout << "MurmurHash3 & " << (float)(end-start)/CLOCKS_PER_SEC << "s \\\\" << endl;

    citywrap cw;
    cw.init();
    start = clock();
    for (uint32_t i = 0; i < trials; ++i)
        x = cw(nums[i]);
    end = clock();
    cout << "CityHash & " << (float)(end-start)/CLOCKS_PER_SEC << "s \\\\" << endl;

    blake2wrap b2;
    b2.init();
    start = clock();
    for (uint32_t i = 0; i < trials; ++i)
        x = b2(nums[i]);
    end = clock();
    cout << "Blake2 & " << (float)(end-start)/CLOCKS_PER_SEC << "s \\\\" << endl;

    simpletab st;
    st.init();
    start = clock();
    for (uint32_t i = 0; i < trials; ++i)
        x = st(nums[i]);
    end = clock();
    cout << "Simple Tabulation & " << (float)(end-start)/CLOCKS_PER_SEC << "s \\\\" << endl;

    twisttab tt;
    tt.init();
    start = clock();
    for (uint32_t i = 0; i < trials; ++i)
        x = tt(nums[i]);
    end = clock();
    cout << "Twisted Tabulation & " << (float)(end-start)/CLOCKS_PER_SEC << "s \\\\" << endl;
}

int main()
{ 
  testTime(2<<25); // 10^7 trials
}
