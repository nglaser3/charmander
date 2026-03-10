#ifndef CHARMANDER_MATERIALS_CE_MATERIAL_H_
#define CHARMANDER_MATERIALS_CE_MATERIAL_H_

#include <vector>

#include "materials/nuclide.h"

namespace charmander
{

  struct NuclideData {
    Nuclide nuc;
    double atom_percent;
  };

  class CEMaterial
  {
  public:
    CEMaterial(int id, const std::vector<NuclideData>& nuclide_data) : id_(id), nuclides_(nuclide_data) {};
  
    int GetID() const {return id_;};
    double GetTotalXS(double energy) const;
    double GetXSFromMT(MT mt, double energy) const;

  private:
    int id_;
    std::vector<NuclideData> nuclides_;
  };
  
} // namespace charmander



#endif // CHARMANDER_MATERIALS_CE_MATERIAL_H_