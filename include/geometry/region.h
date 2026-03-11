#ifndef CHARMANDER_GEOMETRY_REGION_H_
#define CHARMANDER_GEOMETRY_REGION_H_

#include <memory>

#include "basic_types.h"
#include "geometry/surface.h"

namespace charmander
{
  class Halfspace
  {
    public:
      Halfspace(const Surface* surface, bool positive): surface_(surface), positive_(positive) {};

      bool Contains(const Point& p) const {
        return positive_ ? surface_->Sense(p) : !surface_->Sense(p);
      }

      const Surface& GetSurface() const {return *surface_;}

    private:
      const Surface* surface_;
      const bool positive_;
  };
} // namespace charmander


#endif // CHARMANDER_GEOMETRY_REGION_H_