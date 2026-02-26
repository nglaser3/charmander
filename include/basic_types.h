#ifndef CHARMANDER_BASIC_TYPES_H_
#define CHARMANDER_BASIC_TYPES_H_

#include <cmath>
#include <iostream>
#include <limits>

namespace charmander {

constexpr double INF = std::numeric_limits<double>::infinity();

constexpr double FP_TOLERANCE = 1e-12;

constexpr double COINCIDENT_SURF = 1e-12;

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
  Direction(Point p) : x(p.x), y(p.y), z(p.z) {}
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

inline std::ostream& operator<<(std::ostream& os, const Point& p) {
  return os << "Point(" << p.x << ", " << p.y << ", " << p.z << ")";
}

inline bool fuzzyequal(const Point& a, const Point& b) {
  return (a.x - b.x < FP_TOLERANCE) && (a.y - b.y < FP_TOLERANCE) && (a.z - b.z < FP_TOLERANCE);
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

inline double operator*(const Direction& a, const Direction& b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline std::ostream& operator<<(std::ostream& os, const Direction& d) {
  return os << "Direction(" << d.x << ", " << d.y << ", " << d.z << ")";
}

inline Direction normalize(const Direction& d) {
  double norm = std::sqrt(d.x * d.x + d.y * d.y + d.z * d.z);
  return Direction(d.x / norm, d.y / norm, d.z / norm);
}

inline bool fuzzyequal(const Direction& a, const Direction& b) {
  return (a.x - b.x < FP_TOLERANCE) && (a.y - b.y < FP_TOLERANCE) && (a.z - b.z < FP_TOLERANCE);
}

}  // namespace charmander

#endif  // CHARMANDER_BASIC_TYPES_H_