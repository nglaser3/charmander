#ifndef CHARMANDER_GEOMETRY_REGION_H_
#define CHARMANDER_GEOMETRY_REGION_H_

#include <utility>
#include <stdexcept>
#include <vector>

#include "basic_types.h"
#include "geometry/surface.h"

namespace charmander
{
  class Halfspace
  {
    public:
      Halfspace(const Surface* surface, bool positive): surface_(surface), positive_(positive) {
        if (!surface_) throw std::runtime_error("surface not initialized");
      }

      bool Sense(const Point& p) const {
        return positive_ ? surface_->Sense(p) : !surface_->Sense(p);
      }

      const Surface& GetSurface() const {return *surface_;}

      Halfspace operator~() const {return Halfspace(surface_, !positive_);}

    private:
      const Surface* surface_;
      bool positive_;
  };

  class Region
  {
  public:
    Region(std::vector<std::vector<Halfspace>> clauses);
  
    const std::vector<std::vector<Halfspace>>& GetClauses() const {return clauses_;}

    bool Contains(const Point& p) const;

    double Distance(const Point& p, const Direction& d) const;

  private:
    std::vector<std::vector<Halfspace>> clauses_;
  };

  // operator overloads
  inline Halfspace operator+(const Surface& surface) {
    return Halfspace(&surface, true);
  }

  inline Halfspace operator-(const Surface& surface) {
    return Halfspace(&surface, false);
  }

  inline Region operator&(const Halfspace& lhs, const Halfspace& rhs) {
    return Region({{lhs, rhs}});
  }

  inline Region operator|(const Halfspace& lhs, const Halfspace& rhs) {
    return Region({{lhs}, {rhs}});
  }

  inline Region operator&(const Halfspace& lhs, const Region& rhs) {
    auto clauses = rhs.GetClauses();
    for (auto& clause : clauses)
    {
      clause.push_back(lhs);
    }
    return Region(clauses);
  }

  inline Region operator&(const Region& lhs, const Halfspace& rhs) {
    return rhs & lhs;
  }

  inline Region operator|(const Halfspace& lhs, const Region& rhs) {
    auto clauses = rhs.GetClauses();
    clauses.push_back({lhs});
    return Region(clauses);
  }

  inline Region operator|(const Region& lhs, const Halfspace& rhs) {
    return rhs | lhs;
  }

  inline Region operator&(const Region& lhs, const Region& rhs) {
    std::vector<std::vector<Halfspace>> result;
    const auto& lhs_clauses = lhs.GetClauses();
    const auto& rhs_clauses = rhs.GetClauses();

    for (const auto& lhs_clause : lhs_clauses) {
      for (const auto& rhs_clause : rhs_clauses) {
        auto combined = lhs_clause;
        combined.insert(combined.end(), rhs_clause.begin(), rhs_clause.end());
        result.push_back(combined);
      }
    }
    return result;
  }

  inline Region operator|(const Region& lhs, const Region& rhs) {
    auto clauses = lhs.GetClauses();
    const auto& rhs_clauses = rhs.GetClauses();
    clauses.insert(clauses.end(), rhs_clauses.begin(), rhs_clauses.end());
    return Region(clauses);
  }
} // namespace charmander

#endif // CHARMANDER_GEOMETRY_REGION_H_