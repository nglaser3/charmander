#include <stdexcept>

#include "constants.h"
#include "basic_types.h"
#include "geometry/region.h"
#include "geometry/surface.h"

namespace charmander
{
  Region::Region(std::vector<std::vector<Halfspace>> clauses): clauses_(std::move(clauses)) {

    if (clauses_.empty()) throw std::runtime_error("empty clauses vector");
    
    for (auto& clause : clauses) {
      if (clause.empty()) throw std::runtime_error("empty halfspace vector");
    }
  }

  bool Region::Contains(const Point& p) const {
    for (const auto& clause : clauses_) {
      bool inclause = true;
      for (const auto& hs : clause)
      {
        if (!hs.Sense(p)) {
          inclause = false;
          break;
        }
      }
      if (inclause) return true;
    }
    return false;
  }

  double Region::Distance(const Point& p, const Direction& d) const {
    double min_dist = INF;
    for (const auto& clause : clauses_) {
      for (const auto& hs : clause) {
        double dist = hs.GetSurface().Distance(p, d);
        if (dist < min_dist) min_dist = dist;
      }
    }
    return min_dist;
  }
} // namespace charmander
