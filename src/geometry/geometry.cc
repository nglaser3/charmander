#include <utility>

#include "constants.h"
#include "geometry/geometry.h"
#include "materials/nuclide.h"

namespace charmander {

Point Geometry::NewPosition(const Point& p, const Direction& d, double mfp, const double energy) const {
  Point new_p = p;
  while (mfp > 0.0)
  {
    // get next distance to surface
    auto [distance, cell] = DistanceToNextSurface(new_p, d);
    double total_xs = cell.TotalXS(energy);
    if (distance == INF) {
      new_p = {INF, INF, INF};
      mfp = 0.0;
    } else if (mfp < distance * total_xs) {
      new_p += (mfp / total_xs) * d;
      mfp = 0.0;
    } else {
      new_p += distance * d;
      mfp -= distance * total_xs;
    }
  }
  return new_p;
}

inline std::pair<double, const Cell&> Geometry::DistanceToNextSurface(const Point& p, const Direction& d) const {
    double min_distance = INF;
    const Cell* closest_cell = &cells_.front();
    for (const auto& cell : cells_)
    {
      double dist_to_cell = cell.Distance(p, d);
      if (dist_to_cell < min_distance){
        min_distance = dist_to_cell;
        closest_cell = &cell;
      }
    }
    return {min_distance, *closest_cell};
}

MT Geometry::CollisionType(const Point& p, const double energy, double r1, double r2) const {
  for (const auto& cell : cells_)
  {
    if (cell.Contains(p)) return cell.GetFill().SampleReaction(energy, r1, r2);
  }
  return MT::MISSED;
}
}  // namespace charmander