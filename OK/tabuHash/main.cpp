/*
*********************************************************************
*                                                                   *
*                           Open Bloom Filter                       *
*                                                                   *
* Description: Basic Bloom Filter Usage                             *
* Author: Arash Partow - 2000                                       *
* URL: http://www.partow.net                                        *
* URL: http://www.partow.net/programming/hashfunctions/index.html   *
*                                                                   *
* Copyright notice:                                                 *
* Free use of the Open Bloom Filter Library is permitted under the  *
* guidelines and in accordance with the MIT License.                *
* http://www.opensource.org/licenses/MIT                            *
*                                                                   *
*********************************************************************
*/


/*
   Description: This example demonstrates basic usage of the Bloom filter.
                Initially some values are inserted then they are subsequently
                queried, noting any false positives or errors.
*/


#include <iostream>
#include <string>
#include <immintrin.h>
#include <stdio.h>
#include "bloom_filter.hpp"
#include <limits>

int main()
{
  bloom_parameters parameters;
  parameters.projected_element_count = 100000;

  // Maximum tolerable false positive probability? (0,1)
  parameters.false_positive_probability = 0.000001; // 1 in 10000

  // Simple randomizer (optional)
  parameters.random_seed = 0xA5A5A5A5;

  if (!parameters)
    {
      std::cout << "Error - Invalid set of bloom filter parameters!" << std::endl;
      return 1;
    }

  parameters.compute_optimal_parameters();
  __m256i test1;
  
  //Instantiate Bloom Filter
  bloom_filter filter(parameters);

  std::string str_list[] = { "AbC", "iJk", "XYZ" };

  // Insert into Bloom Filter
  {
    // Insert some strings
 
    // Insert some numbers
    for (std::size_t i = 0; i < 1000; ++i)
      {
	filter.insert(i, test1,0);
      }
  }
  /*
  // Query Bloom Filter
  {
    // Query the existence of strings
    for (std::size_t i = 0; i < (sizeof(str_list) / sizeof(std::string)); ++i)
      {
	if (filter.contains(str_list[i]))
	  {
	    std::cout << "BF contains: " << str_list[i] << std::endl;
	  }
      }

    // Query the existence of numbers
    for (std::size_t i = 0; i < 100; ++i)
      {
	if (filter.contains(i))
	  {
	    std::cout << "BF contains: " << i << std::endl;
	  }
      }

    std::string invalid_str_list[] = { "AbCX", "iJkX", "XYZX" };

    // Query the existence of invalid strings
    for (std::size_t i = 0; i < (sizeof(invalid_str_list) / sizeof(std::string)); ++i)
      {
	if (filter.contains(invalid_str_list[i]))
	  {
	    std::cout << "BF falsely contains: " << invalid_str_list[i] << std::endl;
	  }
      }

    // Query the existence of invalid numbers
    for (int i = -1; i > -10000; --i)
      {
	if (filter.contains(i))
	  {
	    std::cout << "BF falsely contains: " << i << std::endl;
	  }
      }
  }
  */
  double result[4];
  uint32_t myarr[8]= {0x10000000,0x00000000,11,0,0,12,0,13};
  __m256i temp = _mm256_loadu_si256((const  __m256i *) &myarr);
  __m256d temp2 = _mm256_castsi256_pd (temp);
  

  // __m256d lowDoubles= _mm256_cvtepi32_pd(_mm256_extracti128_si256(temp1,0));
  // std::cout/* << _mm256_extract_epi64(temp3,0)<<" "<< _mm256_extract_epi64(temp3,1)<<" "<< _mm256_extract_epi64(temp3,7)<<" "*/ <<  temp3[0]<<" "<<temp3[1];
  //  __m256d temp4 = _mm256_cvtepi32_pd(_mm256_castsi256_si128(temp1));
  _mm256_storeu_pd(result, temp2);
  
  std::cout<<result[0]<< " " << result[1]<<std::endl;
  std::cout<<result[2]<<" "<<result[3]<<std::endl;

  
  return 0;


     
}
