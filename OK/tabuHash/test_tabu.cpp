#include "hashing.h"


#include <iostream>
#include <ctime>
#include <vector>
#include <chrono>
#include <immintrin.h>
#include <stdio.h>
#include <random>
using namespace std;

void testTime(int trials)
{
	clock_t end = clock();
	clock_t start = clock();
	mt19937 rng;
     rng.seed(random_device()());
     uniform_int_distribution<uint32_t> dist;
	volatile uint32_t x;
	vector <uint32_t> nums;
    for (uint32_t i = 0; i < trials; ++i){
     // nums.push_back(dist(rng));
      nums.push_back(i+1);
    }
	simpletab st;
    st.init();
    start = clock();
    for (uint32_t i = 0; i < trials; ++i){
      x = st(nums[i]);
	std::cout<<x<<std::endl;	
}
    end = clock();
    cout << "Simple Tabulation & " << (float)(end-start)/CLOCKS_PER_SEC << "s \\\\" << endl;
	
	
	s_simpletab st_1;
    st_1.init(0);
	__m256i test1;
	 __m256i x1;
    start = clock();
    for (uint32_t i = 0; i < trials; ++i)
	{ x1 = st_1(nums[i],test1,0);
	
	}
	    uint32_t output1[8];
    for(uint32_t j =0; j<8; j++){
        output1[j] = _mm256_extract_epi32(x1,j);
        }

        for(uint32_t j = 0; j <8; j++){
        std::cout<<output1[j]<<" "<< j<<std::endl;

        }



    end = clock();
    cout << "Simple Tabulation with SIMD one data 8 different hashing & " << (float)(end-start)/CLOCKS_PER_SEC << "s \\\\" << endl;
	
	s_simpletab st_2;
    st_2.init(1);
    __m256i x2;
    start = clock();
    for (uint32_t j = 0; j < trials; j=j+8)
	{
	uint32_t vecset[8] = {nums[j],nums[j+1],nums[j+2],nums[j+3],nums[j+4],nums[j+5],nums[j+6],nums[j+7]};
    test1 = _mm256_load_si256((__m256i*)vecset);
 
      x2 = st_2(nums[j],test1,1);
	}


        uint32_t output2[8];
    for(uint32_t j =0; j<8; j++){
        output2[j] = _mm256_extract_epi32(x2,j);
        }

        for(uint32_t j = 0; j <8; j++){
        std::cout<<output2[j]<<" "<< j<<std::endl;

        }


    end = clock();
    cout << "Simple Tabulation with SIMD hashing 8 data simult & " << (float)(end-start)/CLOCKS_PER_SEC << "s \\\\" << endl;

}



int main()
{
  testTime(8); // 10^7 trials
}
