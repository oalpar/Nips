#include <iostream>
#include <string>
#include <immintrin.h>
#include <stdio.h>



int main()
{

  __m256i help;
  uint32_t arr[8];
  arr[0] =5;
  arr[1] = 7;
  arr[2] = 12;
  arr[3] = 11;
  arr[4] = 10;
  arr[5] = 20;
  arr[6] = 1;
  arr[7] = 17;

  uint32_t arr2[8];
  arr2[0] =5;
  arr2[1] = 7;
  arr2[2] = 12;
  arr2[3] = 11;
  arr2[4] = 10;
  arr2[5] = 20;
  arr2[6] = 1;
  arr2[7] = 17;
  
  
  help = _mm256_load_si256((__m256i*)arr);
  __m256i temp;
  temp = _mm256_load_si256((__m256i*)arr2);

  
  
  __m256i result;

   _mm256_div_ps(temp,help);
  std::cout<<arr[0]<<" "<<arr[1]<< " " << arr[2] <<  " "<< arr[3]<<std::endl;
  std::cout<<result[0] <<" " <<result[1]<<" "<<result[2] <<" "<< result[3]<<std::endl;
  
  return 0;
}
