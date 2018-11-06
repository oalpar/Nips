#include <iostream>
#include <string>
#include "framework/hashing_more_og.h"
#include <ctime>
#include <vector>
#include <chrono>
#include "framework/hashing.h"
#include <random>
using namespace std;

int main(int argc, char *argv[]){
  volatile uint32_t x;
  volatile __m256i x1;
  
  mt19937 rng;
  rng.seed(random_device()());
  uniform_int_distribution<uint32_t> dist;
  uint32_t trials =2 << stoi(argv[1]);
  vector<uint32_t> nums;
  for (uint32_t i = 0; i < trials; ++i)
    nums.push_back(dist(rng));
 
  
  murmurwrap1 mm;
  
  mm.init();
  clock_t start = clock();
  
  for (uint32_t i = 0; i <trials; ++i){
    x = mm(nums[i]);
    //	cout<<x<<endl;
    }
  clock_t end = clock();
  cout << "MurmurHash3 & " << (float)(end-start)/CLOCKS_PER_SEC << "s \\\\" << endl;
  
  cout<< "Last element with MurmurNoSIMD is " << x << endl;
    
  murmurwrap mmSIMD;
  __m256i murmur;
  const uint32_t* q;
  mmSIMD.init();
  uint32_t xsimd[8];
  uint32_t arr2[8];
    start=clock();
    for(int k=0; k<trials; k+=8){
      arr2[0]=nums[0+k];
      arr2[1]=nums[1+k];
      arr2[2]=nums[2+k];
      arr2[3]=nums[3+k];
      arr2[4]=nums[4+k];
      arr2[5]=nums[5+k];
      arr2[6]=nums[6+k];
      arr2[7]=nums[7+k];
      
      murmur=_mm256_load_si256((__m256i *)arr2);
      x1=mmSIMD(murmur);
      
      q=(const uint32_t *)  & x1;
      
      xsimd[0] = _mm256_extract_epi32(x1, 0);
      xsimd[1] = _mm256_extract_epi32(x1, 1);
      xsimd[2] = _mm256_extract_epi32(x1, 2);
      xsimd[3] = _mm256_extract_epi32(x1, 3);
      xsimd[4] = _mm256_extract_epi32(x1, 4);
      xsimd[5] = _mm256_extract_epi32(x1, 5);
      xsimd[6] = _mm256_extract_epi32(x1, 6);
      xsimd[7] = _mm256_extract_epi32(x1, 7);
      
      //       for(int p=0;p<8;p++){
      //cout <<(x1[p] == nums2[p+i]) << endl;
	 // cout << "Simd: hello "<< xsimd[p] <<endl;
	 // }
    }
    end = clock();
    cout << "MurmurHash3 SIMD & " << (float)(end-start)/CLOCKS_PER_SEC << "s \\\\" << endl;
    cout<< "Last element with MurmurSIMD is " << xsimd[7] << endl;
    return 0;
}




