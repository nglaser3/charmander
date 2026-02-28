#ifndef CHARMANDER_CONSTANTS_H_
#define CHARMANDER_CONSTANTS_H_

#include <limits>

namespace charmander
{
  constexpr double INF = std::numeric_limits<double>::infinity();

  constexpr double FP_TOLERANCE = 1e-12;

  constexpr double COINCIDENT_SURF = 1e-12;
} // namespace charmander

#endif  // CHARMANDER_CONSTANTS_H_