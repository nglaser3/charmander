#include <vector>
#include <stdexcept>
#include <cmath>

#include "constants.h"
#include "materials/ce_material.h"
#include "materials/nuclide.h"

namespace charmander
{
  CEMaterial::CEMaterial(const int id, const std::vector<NuclideData>& nuclide_data, const double density) : id_(id), nuclides_(nuclide_data) {
    // enforce not empty
    if (nuclides_.empty())
    {
      throw std::runtime_error("no nuclides found for material " + std::to_string(id_));
    }

    // enforce loaded nuclides
    for (auto& nucdatum : nuclides_)
    {
      if (!nucdatum.nuc->AlreadyLoaded())
      {
        throw std::runtime_error("non-loaded nuclides detected for material " + std::to_string(id_));
      }
    }
    
    // normalize
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

    // calc number density
    double total_mass = 0.0;
    for (const auto& nucdatum : nuclides_)
        total_mass += nucdatum.atom_percent * nucdatum.mass;

    number_density_ = density * AVOGADRO / total_mass;
  }

  double
  CEMaterial::GetTotalXS(double energy) const {
    float total_xs = 0.0f;
    for (const auto& nucdata : nuclides_)
    {
      size_t lower_energy = nucdata.nuc->GetLowerEnergyBin(energy);
      total_xs += number_density_ * nucdata.atom_percent * nucdata.nuc->GetTotalXS(lower_energy, energy);
    }
    return static_cast<double>(total_xs);
  }

  double
  CEMaterial::GetXSFromMT(MT mt, double energy) const {
    float xs = 0.0f;
    for (const auto& nucdata : nuclides_)
    {
      size_t lower_energy = nucdata.nuc->GetLowerEnergyBin(energy);
      xs += number_density_ * nucdata.atom_percent * nucdata.nuc->GetXSFromMT(mt, lower_energy, energy);
    }
    return static_cast<double>(xs);
  }

  std::pair<MT, double>
  CEMaterial::SampleReaction(double energy, double r1, double r2) const {
    r1 *= GetTotalXS(energy);
    for (const auto& nuc_datum : nuclides_)
    {
      size_t lower_energy = nuc_datum.nuc->GetLowerEnergyBin(energy);
      r1 -= number_density_ * nuc_datum.atom_percent * nuc_datum.nuc->GetTotalXS(lower_energy, energy);
      if (r1 <= 0.0) {
        return {nuc_datum.nuc->SampleReaction(lower_energy, energy, r2), nuc_datum.mass};
      }
    }
    return {MT::MISSED, 1.0};
  }

  double CEMaterial::GetMass() const {
    double total_mass = 0.0;
    for (const auto& nucdatum : nuclides_)
        total_mass += nucdatum.atom_percent * nucdatum.mass;
    return total_mass;
  }
} // namespace charmander
