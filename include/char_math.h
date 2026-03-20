#ifndef CHARMANDER_MATH_H_
#define CHARMANDER_MATH_H_

#include <cmath>
#include <cstdint>
#include "basic_types.h"

namespace charmander
{
  class LinearCongruentialGenerator {
    public: 
      LinearCongruentialGenerator(uint64_t seed): seed_(seed) {}
  
      double operator()() {
        // completely stolen from OpenMC's LCG
        seed_ = 6364136223846793005ULL * seed_ + 1442695040888963407ULL;
        // match actual data size from ull to double
        return double(seed_ >> 11) / double(1ULL << 53);
      }
    private:
      uint64_t seed_;
  }; 

  inline Direction SampleDirection(LinearCongruentialGenerator lcg) {
    double mu = 2.0 * lcg() - 1.0;
    double phi = 2.0 * M_PI * lcg();

    double sin_theta = std::sqrt(1.0 - mu * mu);

    return {sin_theta * std::cos(phi), sin_theta * std::sin(phi), mu};
  }

  inline double SampleMFP(LinearCongruentialGenerator lcg) {
    return -std::log( lcg() );
  }
} // namespace charmander


#endif