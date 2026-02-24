#ifndef CHARMANDER_BASIC_TYPES_H_
#define CHARMANDER_BASIC_TYPES_H_

namespace charmander {

struct Point {
  const double x;
  const double y;
  const double z;
};

struct Direction {
  const double x;
  const double y;
  const double z;
};

}  // namespace charmander

#endif  // CHARMANDER_BASIC_TYPES_H_