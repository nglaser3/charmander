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
    CEMaterial(const int id, const std::vector<NuclideData>& nuclide_data);
  
    const int GetID() const {return id_;}
    const std::vector<NuclideData>& GetNuclides() const {return nuclides_;}
    double GetTotalXS(double energy) const;
    double GetXSFromMT(MT mt, double energy) const;

  private:
    const int id_;
    std::vector<NuclideData> nuclides_;
  };
  
} // namespace charmander



#endif // CHARMANDER_MATERIALS_CE_MATERIAL_H_