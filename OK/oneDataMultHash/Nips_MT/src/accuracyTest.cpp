#include <iostream>
#include <string>
#include "framework/hashing_more_og.h"
#include <ctime>
#include <vector>
#include <chrono>
#include "framework/hashing.h"
#include <random>
using namespace std;

int main(int argc, char** argv){
  volatile uint32_t x;
  volatile __m256i x1;
  
  mt19937 rng;
  rng.seed(random_device()());
  uniform_int_distribution<uint32_t> dist;

  uint64_t ttemp= stoull(argv[1]) ;
uint64_t trials =1<< ttemp/2;
trials =trials << ttemp/2;

if(ttemp %2 != 0){
trials =trials <<1;

}
//std::cout << trials << std::endl;
  vector<uint32_t> nums;
  for (uint64_t i = 0; i < trials; ++i)
    nums.push_back(dist(rng));
 
  
  murmurwrap1 mm;
  
  mm.init();
  clock_t start = clock();
  
  for (uint64_t i = 0; i <trials; ++i){
    x = mm(nums[i]);
    //	cout<<x<<endl;
    }
  clock_t end = clock();
  cout << (float)(end-start)/CLOCKS_PER_SEC << ";" ;
  
  
  murmurwrap mmSIMD;
  __m256i murmur;
  const uint32_t* q;
  mmSIMD.init();
  uint32_t xsimd[8];
  uint32_t arr2[8];
    start=clock();
   /* for(uint64_t k=0; k<trials; k++){
    murmur=  _mm256_set1_epi32 (nums[k]);
      x1=mmSIMD(murmur);
      
      q=(const uint32_t *)  & x1;
        
      //       for(int p=0;p<8;p++){
      //cout <<(x1[p] == nums2[p+i]) << endl;
	 // cout << "Simd: hello "<< xsimd[p] <<endl;
	 // }
    }
    end = clock();
    cout <<  (float)(end-start)/CLOCKS_PER_SEC <<  endl;
     */  return 0;
}




