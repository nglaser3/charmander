#include <vector>
#include <stdexcept>
#include <cmath>

#include "constants.h"
#include "materials/ce_material.h"
#include "materials/nuclide.h"

namespace charmander
{
  CEMaterial::CEMaterial(const int id, const std::vector<NuclideData>& nuclide_data) : id_(id), nuclides_(nuclide_data) {
    if (nuclides_.empty())
    {
      throw std::runtime_error("no nuclides found for material " + std::to_string(id_));
    }
    
    double total_at_percent = 0.0;
    for (auto& nucdatum : nuclides_)
    {
      if (std::signbit( nucdatum.atom_percent))
      {
        throw std::runtime_error("negative atom percent detected on material " + std::to_string(id_));
      }
      total_at_percent += nucdatum.atom_percent;
    }
    for (auto& nucdatum : nuclides_)
    {
      nucdatum.atom_percent /= total_at_percent;
    }
  }

  double
  CEMaterial::GetTotalXS(double energy) const {
    float total_xs = 0.0f;
    for (const auto& nucdata : nuclides_)
    {
      size_t lower_energy = nuclides_.front().nuc->GetLowerEnergyBin(energy);
      total_xs += nucdata.atom_percent * nucdata.nuc->GetTotalXS(lower_energy, energy);
    }
    return static_cast<double>(total_xs);
  }

  double
  CEMaterial::GetXSFromMT(MT mt, double energy) const {
    float xs = 0.0f;
    for (const auto& nucdata : nuclides_)
    {
      size_t lower_energy = nuclides_.front().nuc->GetLowerEnergyBin(energy);
      xs += nucdata.atom_percent * nucdata.nuc->GetXSFromMT(mt, lower_energy, energy);
    }
    return static_cast<double>(xs);
  }
} // namespace charmander
