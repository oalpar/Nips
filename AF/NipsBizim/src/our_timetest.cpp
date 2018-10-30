#include "framework/hashing.h"
#include "framework/hashing_more.h"

#include <iostream>
#include <vector>

#include <random>

#include "common.h"
//#include "generator.h"

#include <omp.h>

int zipf(double alpha, int n)
{
  static int first = 1;      // Static first time flag
  static double c = 0;          // Normalization constant
  static double *sum_probs;     // Pre-calculated sum of probabilities
  double z;                     // Uniform random number (0 < z < 1)
  int zipf_value;               // Computed exponential value to be returned
  int    i;                     // Loop counter
  int low, high, mid;           // Binary-search bounds

  // Compute normalization constant on first call only
  if (first == 1)
    {
      for (i=1; i<=n; i++)
	c = c + (1.0 / pow((double) i, alpha));
      c = 1.0 / c;

      sum_probs = (double*) malloc((n+1)*sizeof(*sum_probs));
      sum_probs[0] = 0;
      for (i=1; i<=n; i++) {
	sum_probs[i] = sum_probs[i-1] + c / pow((double) i, alpha);
      }
      first = 0;
    }

  // Pull a uniform random number (0 < z < 1)
  do
    {
      z = dist(e2);
    }
  while ((z == 0) || (z == 1));

  // Map z to the value
  low = 1, high = n, mid;
  do {
    mid = floor((low+high)/2);
    if (sum_probs[mid] >= z && sum_probs[mid-1] < z) {
      zipf_value = mid;
      break;
    } else if (sum_probs[mid] >= z) {
      high = mid-1;
    } else {
      low = mid+1;
    }
  } while (low <= high);

  // Assert that zipf_value is between 1 and N
  //  assert((zipf_value >=1) && (zipf_value <= n));

  return(zipf_value);
}


void generateData(uint32_t* &data, unsigned long long no_stream, unsigned long long no_unique,int distNum) {
  data = new dtype[no_stream];  

  double max_rand = 0, min_rand = INT_MAX;
  double* rand_data = new double[no_stream];
if(distNum == NORMAL)
{
  //  cout << "normal " << endl;
  double alp = 1.1;
  zipf(alp, no_unique);
#pragma omp parallel num_threads(16)
  {
    int id = omp_get_thread_num();
    /*std::random_device rd; 
    std::mt19937 gen((rd() + id) * (id + 11111)); 
    std::normal_distribution<> ed(normal_dist_mean,normal_dist_var);    */
    
    double my_max = 0;
   #pragma omp for 
    for(stype i = 0; i < no_stream; i++) {
      rand_data[i] = zipf(alp, no_unique);//ed(gen);
      if(rand_data[i] > my_max) {
	my_max = rand_data[i];
      }
    }

    //#pragma omp critical 
    //cout << "veni " << id << endl;

    double my_min = INT_MAX;
#pragma omp for 
    for(stype i = 0; i < no_stream; i++) {
      if(rand_data[i] < my_min) {
	my_min = rand_data[i];
      }
    }
    
#pragma omp critical 
    {
      if(my_max > max_rand) max_rand = my_max;
      if(my_min < min_rand) min_rand = my_min;
    }
  }

#pragma omp parallel for 
  for(stype i = 0; i < no_stream; i++) {
    data[i] = ((rand_data[i] - min_rand) / (max_rand - min_rand)) * (no_unique - 1);
    D(
      if(data[i] >= no_unique || data[i] < 0) {
	std::cout << "ERROR: " << rand_data[i] << " " << min_rand << " " << max_rand << " " << data[i] << std::endl;
	
      }
    );
  }
  /*
  int* counts = new int[no_unique];
  memset(counts, 0, sizeof(int) * no_unique);
  for(stype i = 0; i < no_stream; i++) {
    counts[data[i]]++;
  }
  
  int nz_count = 0, z_count = 0;
  for(int i = 0; i < no_unique; i++) {
    if(counts[i] == 0) {
      z_count++;
    } else {
      nz_count++;
    }
  }

  std::sort(counts, counts + no_unique, std::greater<int>()); 
  for(int i = 0; i < 200; i++) {
    cout << i << ": " << counts[i] << endl;
  }
  cout << "Z: " << z_count << " - NZ: " << nz_count << " - T: " << no_unique << endl;*/
}


void testTime(uint32_t trials, uint32_t* nums)
{

  volatile uint32_t x;
  
  multishift hms;
  hms.init();
  
  double cp1 = omp_get_wtime();
  for(unsigned i = 0; i < trials; i++)
    x = hms(nums[i]);
  double time = omp_get_wtime() - cp1;

  std::cout << "Multiply Shift: " << time << std::endl;





}


int main()
{
  uint32_t* nums;
  generateData(nums, 25, 20, 1);
  testTime(2<<25, nums);  
}
