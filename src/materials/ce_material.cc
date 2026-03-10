#include <vector>

#include "materials/ce_material.h"
#include "materials/nuclide.h"

namespace charmander
{
  double
  CEMaterial::GetTotalXS(double energy) const {
    double lower_energy = nuclides_.front().nuc.GetLowerEnergyBin(energy);

    float total_xs = 0.0f;
    for (auto nucdata : nuclides_)
    {
      total_xs += nucdata.atom_percent * nucdata.nuc.GetTotalXS(lower_energy, energy);
    }
    return static_cast<double>(total_xs);
  }

  double
  CEMaterial::GetXSFromMT(MT mt, double energy) const {
    double lower_energy = nuclides_.front().nuc.GetLowerEnergyBin(energy);

    float xs = 0.0f;
    for (auto nucdata : nuclides_)
    {
      xs += nucdata.atom_percent * nucdata.nuc.GetXSFromMT(mt, lower_energy, energy);
    }
    return static_cast<double>(xs);
  }
} // namespace charmander
