#ifndef CHARMANDER_BASIC_TYPES_H_
#define CHARMANDER_BASIC_TYPES_H_

#include <limits>

namespace charmander {

constexpr double INF = std::numeric_limits<double>::infinity();

// ----------------------------------------------------------------------------
// Point
// ----------------------------------------------------------------------------

struct Point {
  const double x;
  const double y;
  const double z;

  Point(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
};

// ----------------------------------------------------------------------------
// Direction
// ----------------------------------------------------------------------------

struct Direction {
  const double x;
  const double y;
  const double z;
  Direction(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
};

// ----------------------------------------------------------------------------
// Helper operator overloads
// ----------------------------------------------------------------------------

inline bool operator==(const Point& a, const Point& b) {
  return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline Point operator+(const Point& a, const Point& b) {
  return Point(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Point operator-(const Point& a, const Point& b) {
  return Point(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Point operator+(const Point& p, const Direction& d) {
  return Point(p.x + d.x, p.y + d.y, p.z + d.z);
}

inline Point operator-(const Point& p, const Direction& d) {
  return Point(p.x - d.x, p.y - d.y, p.z - d.z);
}

inline bool operator==(const Direction& a, const Direction& b) {
  return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline Direction operator+(const Direction& a, const Direction& b) {
  return Direction(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Direction operator-(const Direction& a, const Direction& b) {
  return Direction(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Direction operator-(const Direction& d) {
  return Direction(-d.x, -d.y, -d.z);
}

inline Direction operator*(const double& a, const Direction& d) {
  return Direction(a * d.x, a * d.y, a * d.z);
}

}  // namespace charmander

#endif  // CHARMANDER_BASIC_TYPES_H_