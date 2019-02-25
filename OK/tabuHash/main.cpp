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

#include "bloom_filter.hpp"
#include "hashing.h"
int main()
{
  simpletab st;
  st.init();
  bloom_parameters parameters;

  // How many elements roughly do we expect to insert?
  parameters.projected_element_count = 1000;

  // Maximum tolerable false positive probability? (0,1)
  parameters.false_positive_probability = 0.0001; // 1 in 10000

  // Simple randomizer (optional)
  parameters.random_seed = 0xA5A5A5A5;

  if (!parameters)
    {
      std::cout << "Error - Invalid set of bloom filter parameters!" << std::endl;
      return 1;
    }

  parameters.compute_optimal_parameters();

  //Instantiate Bloom Filter
  bloom_filter filter(parameters);

  std::string str_list[] = { "AbC", "iJk", "XYZ" };

  // Insert into Bloom Filter
  
    // Insert some strings
    /* for (std::size_t i = 0; i < (sizeof(str_list) / sizeof(std::string)); ++i)
      {
	std::cout <<i<<std::endl;
	filter.insert(str_list[i]);
      } */

    // Insert some numbers
    for (std::size_t i = 0; i < 2; ++i)
      {
	int x= st(i*10000);
	int index = x % filter.table_size_;
	int bit = x % 0x08;
	//bit_index =st(temp);
	  //bit_index = bit_index % table_size_;
	//bit       = bit_index % bits_per_char;
	std::cout <<filter.table_size_<<" "<<0x08<<" "<<index <<std::endl;
	filter.insert(index,bit);
      }
  

  // Query Bloom Filter
  {
    // Query the existence of strings
    /*  for (std::size_t i = 0; i < (sizeof(str_list) / sizeof(std::string)); ++i)
      {
	if (filter.contains(str_list[i]))
	  {
	    std::cout << "BF contains: " << str_list[i] << std::endl;
	  }
      }
*/
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
    for (std::size_t i = 0; i < 1000000; ++i)
      {
	if (filter.contains(i))
	  {
	    //	    std::cout << "BF falsely contains: " << i << std::endl;
	  }
      }

    // Query the existence of invalid numbers
    for (int i = -1; i > -100; --i)
      {
	if (filter.contains(i))
	  {
	    //    std::cout << "BF falsely contains: " << i << std::endl;
	  }
      }
  }

  return 0;
}