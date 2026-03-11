#ifndef CHARMANDER_GEOMETRY_REGION_H_
#define CHARMANDER_GEOMETRY_REGION_H_

#include <cassert>
#include <utility>
#include <vector>

#include "basic_types.h"
#include "geometry/surface.h"

namespace charmander
{
  class Halfspace
  {
    public:
      Halfspace(const Surface* surface, bool positive): surface_(surface), positive_(positive) {assert(surface_ != nullptr);}

      bool Sense(const Point& p) const {
        return positive_ ? surface_->Sense(p) : !surface_->Sense(p);
      }

      const Surface& GetSurface() const {return *surface_;}

      Halfspace operator~() const {return Halfspace(surface_, !positive_);}

    private:
      const Surface* surface_;
      const bool positive_;
  };

  class Region
  {
  public:
    Region(std::vector<Halfspace> halfspaces): halfspaces_(std::move(halfspaces)) {}
  
    const std::vector<Halfspace>& GetHalfspaces() const {return halfspaces_;}

    bool Contains(const Point& p) const {
      for (const auto& hs : halfspaces_) if (!hs.Sense(p)) return false;
      return true;
    };

  private:
    std::vector<Halfspace> halfspaces_;
  };

  // operator overloads
  inline Halfspace operator+(const Surface& surface) {
    return Halfspace(&surface, true);
  }

  inline Halfspace operator-(const Surface& surface) {
    return Halfspace(&surface, false);
  }

  inline Region operator&(const Halfspace& lhs, const Halfspace& rhs) {
    return Region({lhs, rhs});
  }

  inline Region operator&(const Halfspace& lhs, const Region& rhs) {
    auto halfspaces = rhs.GetHalfspaces();
    halfspaces.push_back(lhs);
    return Region(halfspaces);
  }

  inline Region operator&(const Region& lhs, const Halfspace& rhs) {
    return rhs & lhs;
  }

  inline Region operator&(const Region& lhs, const Region& rhs) {
    auto hs = lhs.GetHalfspaces();
    const auto& rhs_hs = rhs.GetHalfspaces();
    hs.insert(hs.end(), rhs_hs.begin(), rhs_hs.end());
    return Region(hs);
  }
  
} // namespace charmander



#endif // CHARMANDER_GEOMETRY_REGION_H_