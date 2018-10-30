#include "framework/hashing.h"
#include "framework/testHash.h"
#include "framework/hashing_more.h"

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

    volatile uint32_t x2;
    mt19937 rng;
    rng.seed(random_device()());
    uniform_int_distribution<uint32_t> dist;
    vector<uint32_t> nums;
    vector<uint32_t> nums2;
    for (uint32_t i = 0; i < trials; ++i){
      nums.push_back(dist(rng));
     
     }
     org_multishift hms2;
     hms2.init();
     clock_t start2 = clock();
     
     for (uint32_t i = 0; i < trials; i+=1)
       {
	 x2=hms2(nums[i]);
	 nums2.push_back((uint32_t)x2);
       }

     clock_t end2 = clock();
     cout << "Original Multiply-shift & " << (float)(end2-start2)/CLOCKS_PER_SEC << "s \\\\" << endl;
     
     __m256i values;
     int arr1[8];
     _mm256_store_si256((__m256i *) arr1, values);
     multishift hms;
     
     hms.init();
     trials=trials-trials%8;

     clock_t start = clock();

     for (uint32_t i = 0; i < trials; i+=8)
       { 
	 int arr2[8]={nums[i], nums[i+1], nums[i+2],
		      nums[i+3],nums[i+4],nums[i+5],
		      nums[i+6],nums[i+7]} ;
	 values= _mm256_load_si256((__m256i*) arr2); 
	 x1=hms(values);
	 for(int p=0;p<8;p++){
	   cout <<(x1[p] == nums2[p+i]) << endl;
	
	 }
	 break;
       }
     clock_t end = clock();
     cout << "Multiply-shift & " << (float)(end-start)/CLOCKS_PER_SEC << "s \\\\" << endl;
     /*****************************************************************************************************/
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
