#include "materials/nuclide.h"

#include <algorithm>
#include <iterator>

#include "materials/xs_file_interface.h"

namespace charmander {

void Nuclide::LoadFromFile() {
  // open xs file
  XSFileInterface xs_file(nuclide_name_);

  // energies
  xs_file.LoadEvaluationEnergies(temperature_, evaluation_energies_);

  // fill the xs
  total_xs_.assign(evaluation_energies_.size(),
                   0.0f);  // temporary before adding an actual function
  xs_file.Load1DXSDataset("002", temperature_, xs_map_[MT::ELASTIC],
                          evaluation_energies_.size());
  xs_file.LeftPadLoad1DXSDataset("004", temperature_, xs_map_[MT::INELASTIC],
                                 evaluation_energies_.size());
  xs_file.Load1DXSDataset("018", temperature_, xs_map_[MT::FISSION],
                          evaluation_energies_.size());
  xs_file.Load1DXSDataset("102", temperature_, xs_map_[MT::CAPTURE],
                          evaluation_energies_.size());

  // calculate the total xs from the above
  ConstructTotalXS(total_xs_);
};

void Nuclide::ConstructTotalXS(std::vector<float> total_xs) const {
  size_t length = evaluation_energies_.size();
  for (const auto& [mt, xs] : xs_map_) {
    for (size_t i = 0; i < length; ++i) {
      total_xs[i] = xs[i];
    }
  }
};

size_t Nuclide::GetLowerEnergyBin(double energy) const {
  const size_t size_of_energies = evaluation_energies_.size();
  const double* energies = evaluation_energies_.data();

  if (energy <= energies[0]) return 0;
  if (energy >= energies[size_of_energies - 1]) return size_of_energies - 2;

  auto it = std::lower_bound(energies, energies + size_of_energies, energy);
  return static_cast<size_t>(it - energies) - 1;
}

double Nuclide::GetTotalXS(const size_t energy_index,
                           const double energy) const {
  if (energy <= evaluation_energies_.front()) return total_xs_.front();
  if (energy >= evaluation_energies_.back()) return total_xs_.back();

  const double* energies = evaluation_energies_.data();
  const float* xs = total_xs_.data();

  double E_low = energies[energy_index];
  double E_high = energies[energy_index + 1];

  double XS_low = xs[energy_index];
  double XS_high = xs[energy_index + 1];

  return XS_low + (XS_high - XS_low) * (energy - E_low) / (E_high - E_low);
}

double Nuclide::GetXSFromMT(MT mt, const size_t energy_index,
                            const double energy) const {
  const float* xs = xs_map_.at(mt).data();
  if (energy <= evaluation_energies_.front()) return xs[0];
  if (energy >= evaluation_energies_.back())
    return xs[evaluation_energies_.size() - 1];

  const double* energies = evaluation_energies_.data();

  double E_low = energies[energy_index];
  double E_high = energies[energy_index + 1];

  double XS_low = xs[energy_index];
  double XS_high = xs[energy_index + 1];

  return XS_low + (XS_high - XS_low) * (energy - E_low) / (E_high - E_low);
}
}  // namespace charmander
