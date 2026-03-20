#include <gtest/gtest.h>

#include "char_math.h"

namespace charmander
{
  TEST(CharmanderMath, LinearCongruentialGenerator) {
    LinearCongruentialGenerator lcg(1);
    for (size_t i = 0; i < 100000; i++)
    {
      double rand = lcg();
      EXPECT_LE(rand, 1.0);
      EXPECT_GE(rand, 0.0);
    }
  }
} // namespace charmander
