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
    const Cell* cell = FindCell(new_p);
    if (!cell) return INF_POINT;

    double distance = cell->Distance(new_p, d);

    if (distance == INF) return INF_POINT;

    double total_xs = cell->TotalXS(energy);
    if (mfp < distance * total_xs) {
      new_p += (mfp / total_xs) * d;
      mfp = 0.0;
    } else {
      new_p += distance * d;
      mfp -= distance * total_xs;
    }
  }
  return new_p;
}

std::pair<double, const Cell*> Geometry::DistanceToNextSurface(const Point& p, const Direction& d) const {
    double min_distance = INF;
    const Cell* closest_cell = nullptr;
    for (const auto& cell : cells_)
    {
      double dist_to_cell = cell.Distance(p, d);
      if (dist_to_cell < min_distance){
        min_distance = dist_to_cell;
        closest_cell = &cell;
      }
    }
    return {min_distance, closest_cell};
}

std::pair<MT, double> Geometry::CollisionType(const Point& p, const double energy, double r1, double r2) const {
  for (const auto& cell : cells_)
  {
    if (cell.Contains(p)) return cell.GetFill().SampleReaction(energy, r1, r2);
  }
  return {MT::MISSED, 1.0};
}

double Geometry::ProbabilityNonAbs(const Point& p, const double energy) const {
  for (const auto& cell : cells_)
  {
    if (cell.Contains(p)) {
      auto& fill = cell.GetFill();
      double total_xs = fill.GetTotalXS(energy);
      double abs_xs = fill.GetXSFromMT(MT::CAPTURE, energy) + fill.GetXSFromMT(MT::FISSION, energy);
      return 1 - abs_xs / total_xs;
    }
  }
  return 0.0;  
}

double Geometry::GetMass(const Point& p) const {
  for (const auto& cell : cells_)
  {
    if (cell.Contains(p)) return cell.GetFill().GetMass();
  }
  return 0.0;
}

  const Cell* Geometry::FindCell(const Point& p) const {
    for (const auto& cell : cells_) {
      if (cell.Contains(p)) return &cell;
    }
    return nullptr;
  }
}  // namespace charmander