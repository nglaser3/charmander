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
  xs_file.Load1DXSDataset("002", temperature_, elastic_xs_);
  xs_file.Load1DXSDataset("004", temperature_, inelastic_xs_);
  xs_file.Load1DXSDataset("018", temperature_, fission_xs_);
  xs_file.Load1DXSDataset("102", temperature_, capture_xs_);

  // close file
};

} // namespace charmander
