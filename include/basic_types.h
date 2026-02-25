#ifndef CHARMANDER_BASIC_TYPES_H_
#define CHARMANDER_BASIC_TYPES_H_

#include <limits>

namespace charmander {

constexpr double INF = std::numeric_limits<double>::infinity();

struct Point {
  const double x;
  const double y;
  const double z;

  Point(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
};

inline bool operator==(const Point& a, const Point& b) {
  return a.x == b.x && a.y == b.y && a.z == b.z;
}

struct Direction {
  const double x;
  const double y;
  const double z;
  Direction(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
};

inline bool operator==(const Direction& a, const Direction& b) {
  return a.x == b.x && a.y == b.y && a.z == b.z;
}

}  // namespace charmander

#endif  // CHARMANDER_BASIC_TYPES_H_