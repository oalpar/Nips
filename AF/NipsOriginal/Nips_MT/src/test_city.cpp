#include "framework/hashing.h"
#include "framework/hashing_more.h"
#include <immintrin.h>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <vector>
#include <chrono>
#include <fstream>
#include <random>
#include <string>
using namespace std;
void multdata_1hash(uint32_t trials, string t)
{
	ofstream file;
	file.open (t);
	 mt19937 rng;
     rng.seed(random_device()());
     uniform_int_distribution<uint32_t> dist;
	volatile uint32_t x;
	vector<uint32_t> nums;
    for (uint32_t i = 0; i < trials; ++i){
      nums.push_back(dist(rng));
      
    }
	s_citywrap scw;
	scw.init(1);
	 clock_t end = clock();
	clock_t start = clock();
    start = clock();
	__m256i vec1;
    for(int j = 0; j < trials; j+=4) {
     uint64 vecset1[4] = {nums[j],nums[j+1],nums[j+2],nums[j+3]};
     vec1 = _mm256_loadu_si256((__m256i*)vecset1);
     __m256i test1=scw(vec1);
         }
	end = clock();
    file << "CityHash&simd \n " << (float)(end-start)/CLOCKS_PER_SEC << ",\n\n" << endl;

    citywrap cw;
    cw.init();
    start = clock();
    for (uint32_t i = 0; i < trials; ++i)
      x = cw(nums[i]);
	
    end = clock();
    file << "CityHash \n" << (float)(end-start)/CLOCKS_PER_SEC << ",\n\n" << endl;
}
void onedata_multhash(uint32_t trials,string t)
{
	ofstream file;
	file.open (t);
  mt19937 rng;
  rng.seed(random_device()());
  uniform_int_distribution<uint32_t> dist;
  volatile uint32_t x;
  vector<uint32_t> nums;
    
    for (uint32_t i = 0; i < trials; ++i){
      nums.push_back(dist(rng));
      
    }
	 clock_t end = clock();
	clock_t start = clock();	
	s_citywrap scw;
	scw.init(0);
	uint64 *hashed1 = new uint64[trials];
    
	start = clock();
	__m256i vec1;
	//NUMBER OF HASHES =4
    for(int j = 0; j < trials; j+=1) {
     uint64 vecset1[4] = {nums[j],nums[j],nums[j],nums[j]};
     vec1 = _mm256_loadu_si256((__m256i*)vecset1);	
     __m256i test1=scw(vec1);
         }
	end = clock();
    file << "CityHash&simd \n " << (float)(end-start)/CLOCKS_PER_SEC << ",\n" << endl;
	start = clock();
	for(int=0; i<4;i++){
    citywrap cw;
    cw.init();
    for (uint32_t i = 0; i < trials; ++i)
      x = cw(nums[i]);
    }
	end = clock();
    file << "CityHash \n" << (float)(end-start)/CLOCKS_PER_SEC << ",\n" << endl;
}
int main()
{
  
  //for(int i=0; i<100; i++)
  //{
  multdata_1hash(2<<25,"multdata_onehash25.txt");
  multdata_1hash(2<<30,"multdata_onehash30.txt");
  onedata_multhash(2<<25 ,"onedata_multhash25.txt");
  onedata_multhash(2<<30 ,"onedata_multhash30.txt");
  //}
}
