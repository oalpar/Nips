#include "framework/hashing.h"

#include "framework/hashing_more.h"

#include <immintrin.h>
#include <stdio.h>

#include <iostream>
#include <ctime>
#include <vector>
#include <chrono>

#include <random>

using namespace std;


void testTime(uint32_t trials)
{
	mt19937 rng;
    rng.seed(random_device()());
	uniform_int_distribution<uint32_t> dist;
	uint32_t ra1=dist(rng);
	uint32_t rb1= dist(rng);
	
    volatile __m256i x1;
    volatile uint32_t x;
	multishift hms;
	hms.init(ra1, rb1);
    volatile uint32_t x2;
    
    
    vector<uint32_t> nums;
    vector<uint32_t> nums2;
    vector<uint32_t> nums3;
    for (uint32_t i = 0; i < trials; ++i){
      nums.push_back(dist(rng));
     
     }
     org_multishift hms2;
     hms2.init(ra1,rb1);
     clock_t start2 = clock();
     
     for (uint32_t i = 0; i < trials; i+=1)
       {
	 x2=hms2(nums[i]);
	 nums2.push_back((uint32_t)x2);
       }

     clock_t end2 = clock();
     cout << "Original Multiply-shift: " << (float)(end2-start2)/CLOCKS_PER_SEC << "s \\\\" << endl;
     
     //__m256i values;
     //int arr1[8];
     //_mm256_store_si256((__m256i *) arr1, values);
     //multishift hms;
     
     //hms.init();
     //trials=trials-trials%8;

     //clock_t start = clock();

     //for (uint32_t i = 0; i < trials; i+=8)
     //  { 
     //	 int arr2[8]={nums[i], nums[i+1], nums[i+2],
     //		      numsnums[i+3],nums[i+4],nums[i+5],
     //		      nums[i+6],nums[i+7]} ;
     //	 values= _mm256_load_si256((__m256i*) arr2); 
     //	 x1=hms(values);
     //	 int xsimd[8];
     //	 xsimd[0] = _mm256_extract_epi32(x1, 0);
     //	 xsimd[1] = _mm256_extract_epi32(x1, 1);
     //	 xsimd[2] = _mm256_extract_epi32(x1, 2);
     //	 xsimd[3] = _mm256_extract_epi32(x1, 3);
     //	 xsimd[4] = _mm256_extract_epi32(x1, 4);
     //	 xsimd[5] = _mm256_extract_epi32(x1, 5);
     //	 xsimd[6] = _mm256_extract_epi32(x1, 6);
     //	 xsimd[7] = _mm256_extract_epi32(x1, 7);
     //	 for(int p=0;p<8;p++){
     //	   //cout <<(x1[p] == nums2[p+i]) << endl;
     //	   cout << "Real: " << nums2[p] << " Simd: "<< xsimd[p] <<endl;// << " Simd: " << x1[p] << endl;
     //	 }
     //	 break;
     //}
     //clock_t end = clock();
     //cout << "Multiply-shift & " << (float)(end-start)/CLOCKS_PER_SEC << "s \\\\" << endl;
     /*****************************************************************************************************/

     //org_multishift simdshift;
     //simdshift.init();

     uint32_t *hashed = new uint32_t[trials];
     //uint32_t hashedc[8];
     //uint32_t hashed;
     
     clock_t start = clock();
     for(int j = 0; j < trials; j+=8) {
     __m256i vec;
     uint32_t vecset[8] = {nums[j],nums[j+1],nums[j+2],nums[j+3],nums[j+4],nums[j+5],nums[j+6],nums[j+7]};
     
     
     vec = _mm256_load_si256((__m256i*)vecset);
     __m256i test=hms(vec);
     for(int k = 0; k < 8; k++) {
       hashed[j+k] = _mm256_extract_epi32(test,k);
       //cout << "Index: " <<j+k << " Val: " << hashed[j+k] << endl; 
       //hashedc[k] = hms2(_mm256_extract_epi32(vec,k));
     }	  

     //for(int k = 0; k < 8; k++) {
     // nums3.push_back(hashed[k]);
     //}
     
     /*if(j < -1){
       uint32_t xsimd[8];
       
       xsimd[0] = _mm256_extract_epi32(vec, 0);
       xsimd[1] = _mm256_extract_epi32(vec, 1);
       xsimd[2] = _mm256_extract_epi32(vec, 2);
       xsimd[3] = _mm256_extract_epi32(vec, 3);
       xsimd[4] = _mm256_extract_epi32(vec, 4);
       xsimd[5] = _mm256_extract_epi32(vec, 5);
       xsimd[6] = _mm256_extract_epi32(vec, 6);
       xsimd[7] = _mm256_extract_epi32(vec, 7);
       }*/
     
     }

     clock_t end = clock();
     
     for(int i = 0; i < 100; i++) {
	 cout << "Real: " << nums[i] << " \tOriginal: " << nums2[i] << " \tSimd: " << hashed[i] << endl;
       }
     
     
     cout << "Simd Multiply-shift: " << (float)(end-start)/CLOCKS_PER_SEC << "s \\\\" << endl;
     
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
    for (uint32_t i = 0; i < trials; ++i)
      x = mm(nums[i]);
    end = clock();
    cout << "MurmurHash3 & " << (float)(end-start)/CLOCKS_PER_SEC << "s \\\\" << endl;
    
    /*   citywrap cw;
	 cw.init();
	 start = clock();
	 for (uint32_t i = 0; i < trials; ++i)
	 x = cw(nums[i]);
	 end = clock();
	 cout << "CityHash & " << (float)(end-start)/CLOCKS_PER_SEC << "s \\\\" << endl;
    */
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
