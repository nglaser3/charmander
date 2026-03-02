#include "materials/xs_file_interface.h"
#include "materials/nuclide.h"

namespace charmander
{
  
void Nuclide::LoadFromFile() {

  // open xs file
  XSFileInterface xs_file(nuclide_name_);

  // energies 
  xs_file.LoadEvaluationEnergies(temperature_, evaluation_energies_);

  // fill the xs
  total_xs_.assign(evaluation_energies_.size(), 0.0f); //temporary before adding an actual function
  xs_file.Load1DXSDataset("002", temperature_, elastic_xs_, evaluation_energies_.size());
  xs_file.LeftPadLoad1DXSDataset("004", temperature_, inelastic_xs_, evaluation_energies_.size());
  xs_file.Load1DXSDataset("018", temperature_, fission_xs_, evaluation_energies_.size());
  xs_file.Load1DXSDataset("102", temperature_, capture_xs_, evaluation_energies_.size());

  // calculate the total xs from the above
  ConstructTotalXS(total_xs_);
};

void Nuclide::ConstructTotalXS(std::vector<float>& total_xs) const {
  size_t length = evaluation_energies_.size();
  for (size_t i = 0; i < length; ++i) {
    total_xs[i] = elastic_xs_[i] + inelastic_xs_[i] + fission_xs_[i] + capture_xs_[i];
  }
}
} // namespace charmander
