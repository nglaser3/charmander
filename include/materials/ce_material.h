#ifndef CHARMANDER_MATERIALS_CE_MATERIAL_H_
#define CHARMANDER_MATERIALS_CE_MATERIAL_H_

#include <memory>
#include <utility>
#include <vector>

#include "materials/nuclide.h"

namespace charmander
{

  struct NuclideData {
    std::shared_ptr<const Nuclide> nuc; 
    double atom_percent;
    double mass = 1.0;
  };

  class CEMaterial
  {
  public:
    CEMaterial(const int id, const std::vector<NuclideData>& nuclide_data);
  
    const int GetID() const {return id_;}
    const std::vector<NuclideData>& GetNuclides() const {return nuclides_;}
    double GetTotalXS(double energy) const;
    double GetXSFromMT(MT mt, double energy) const;
    std::pair<MT, double> SampleReaction(double energy, double r1, double r2) const;

  private:
    const int id_;
    std::vector<NuclideData> nuclides_;
  };
  
} // namespace charmander



#endif // CHARMANDER_MATERIALS_CE_MATERIAL_H_