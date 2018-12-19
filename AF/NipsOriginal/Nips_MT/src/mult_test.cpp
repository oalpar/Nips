#include "framework/hashing.h"

#include "framework/hashing_more.h"

#include <immintrin.h>
#include <stdio.h>

#include <iostream>
#include <ctime>
#include <vector>
#include <chrono>

#include <random>


int main()
{
  testTime(2<<25); // 10^7 trials
}