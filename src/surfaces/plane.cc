#include "surfaces/plane.h"

#include <cmath>

#include "basic_types.h"

namespace charmander {
Plane::Plane(double a, double b, double c, double d)
    : Surface(), a_(a), b_(b), c_(c), d_(d) {};

double Plane::Evaluate(Point p) const {
  return a_ * p.x + b_ * p.y + c_ * p.z - d_;
};

Direction Plane::Normal(Point p) const { return normalize({a_, b_, c_}); };

double Plane::Distance(Point p, Direction d) const {
  double denominator = (d.x * a_ + d.y * b_ + d.z * c_);
  if (std::abs(denominator) < FP_TOLERANCE) return INF;

  double distance = -Evaluate(p) / denominator;
  return (distance > COINCIDENT_SURF) ? distance : INF;
};
}  // namespace charmander
