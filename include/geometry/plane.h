#ifndef CHARMANDER_GEOMETRY_PLANE_H_
#define CHARMANDER_GEOMETRY_PLANE_H_

#include "basic_types.h"
#include "geometry/surface.h"

namespace charmander {

class Plane : public Surface {
 public:
  Plane(double a, double b, double c, double d);

  virtual double Evaluate(Point p) const override;

  virtual Direction Normal(Point p) const override;

  virtual double Distance(Point p, Direction d) const override;

 protected:
  double a_;
  double b_;
  double c_;
  double d_;
};

class XPlane : public Plane {
 public:
  XPlane(double x0) : Plane(1.0, 0.0, 0.0, x0) {};
};

class YPlane : public Plane {
 public:
  YPlane(double y0) : Plane(0.0, 1.0, 0.0, y0) {};
};

class ZPlane : public Plane {
 public:
  ZPlane(double z0) : Plane(0.0, 0.0, 1.0, z0) {};
};

}  // namespace charmander

#endif  // CHARMANDER_GEOMETRY_PLANE_H_