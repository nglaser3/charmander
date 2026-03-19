#ifndef CHARMANDER_GEOMETRY_CELL_H_
#define CHARMANDER_GEOMETRY_CELL_H_

#include "basic_types.h"
#include "materials/ce_material.h"
#include "geometry/region.h"

namespace charmander
{
  class Cell
  {
    public:
      Cell(const int uid, const CEMaterial& fill, const Region& region): uid_(uid), fill_(fill), region_(region) {}

      bool Contains(const Point& p) const {return region_.Contains(p);}

      double Distance(const Point& p, const Direction& d) const {return region_.Distance(p, d);}

      double TotalXS(const double& energy) const {return fill_.GetTotalXS(energy);}

      int GetUID() const {return uid_;}

      const CEMaterial& GetFill() const {return fill_;}

      const Region& GetRegion() const {return region_;}

    private:
      const int uid_; 
      const CEMaterial fill_;
      const Region region_;
  };
} // namespace charmander

#endif  // CHARMANDER_GEOMETRY_CELL_H_