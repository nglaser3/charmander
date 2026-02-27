#ifndef CHARMANDER_GEOMETRY_CYLINDER_H_
#define CHARMANDER_GEOMETRY_CYLINDER_H_

#include "basic_types.h"
#include "geometry/surface.h"

namespace charmander {

class Cylinder : public Surface {
 public:
  Cylinder(double r, Direction axis, Point center);

  double Evaluate(Point p) const override;

  Direction Normal(Point p) const override;

  double Distance(Point p, Direction d) const override;

 protected:
  double r_;
  Direction axis_;
  Point p0_;
};

class XCylinder : public Cylinder {
 public:
  XCylinder(double r, Point center)
      : Cylinder(r, Direction(1.0, 0.0, 0.0), Point(0.0, center.y, center.z)) {}
};

class YCylinder : public Cylinder {
 public:
  YCylinder(double r, Point center)
      : Cylinder(r, Direction(0.0, 1.0, 0.0), Point(center.x, 0.0, center.z)) {}
};

class ZCylinder : public Cylinder {
 public:
  ZCylinder(double r, Point center)
      : Cylinder(r, Direction(0.0, 0.0, 1.0), Point(center.x, center.y, 0.0)) {}
};

}  // namespace charmander

#endif  // CHARMANDER_GEOMETRY_CYLINDER_H_