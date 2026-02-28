#include "geometry/cylinder.h"

#include "constants.h"
#include "basic_types.h"

namespace charmander {
Cylinder::Cylinder(double r, Direction axis, Point center)
    : r_(r), axis_(normalize(axis)), p0_(center) {}

double Cylinder::Evaluate(Point p) const {
  Direction w = p - p0_;
  double wu = w * axis_;
  Direction wperp = w - wu * axis_;
  return wperp * wperp - r_ * r_;
}

Direction Cylinder::Normal(Point p) const {
  Direction w = p - p0_;
  double wu = w*axis_;
  Direction n = w - wu*axis_;
  return normalize(n);
}

double Cylinder::Distance(Point p, Direction d) const {
  Direction w = p - p0_;
  double du = d * axis_;
  double wu = w * axis_;

  Direction dperp = d - (du * axis_);
  Direction wperp = w - (wu * axis_);

  double A = dperp * dperp;
  double B = 2 * wperp * dperp;
  double C = wperp * wperp - r_ * r_;

  if (std::abs(A) < FP_TOLERANCE) return INF;

  double discriminant = B * B - 4 * A * C;

  if (discriminant < 0.0) return INF;

  double sqrt_disc = std::sqrt(discriminant);
  double r1 = (-B - sqrt_disc) / (2 * A);
  double r2 = (-B + sqrt_disc) / (2 * A);

  double distance = INF;
  if (r1 > COINCIDENT_SURF) distance = r1;
  if (r2 > COINCIDENT_SURF && r2 < distance) distance = r2;
  return distance;
}
}  // namespace charmander
