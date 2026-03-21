#ifndef CHARMANDER_GEOMETRY_GEOMETRY_H_
#define CHARMANDER_GEOMETRY_GEOMETRY_H_

#include <vector>
#include <utility>

#include "materials/nuclide.h"
#include "basic_types.h"
#include "geometry/cell.h"

namespace charmander {

class Geometry {
 public:
  Geometry(const std::vector<Cell>& cells): cells_(cells) {}

  Point NewPosition(const Point& p, const Direction& d, const double mfp, const double energy) const;

  std::pair<double, const Cell*> DistanceToNextSurface(const Point& p, const Direction& d) const;

  std::pair<MT, double> CollisionType(const Point& p, const double energy, double r1, double r2) const;

  double ProbabilityNonAbs(const Point& p, const double energy) const;

  double GetMass(const Point& p) const; 

  const Cell* FindCell(const Point& p) const;
 private:
  const std::vector<Cell> cells_;
};

}  // namespace charmander

#endif  // CHARMANDER_GEOMETRY_GEOMETRY_H