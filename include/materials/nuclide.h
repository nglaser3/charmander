#ifndef CHARMANDER_MATERIALS_NUCLIDE_H_
#define CHARMANDER_MATERIALS_NUCLIDE_H_

#include <string>
#include <vector>

namespace charmander
{
  
class Nuclide
{
  public:
    Nuclide(std::string nuclide): nuclide_name_(nuclide){};
    void LoadFromFile();
  private:
    std::string nuclide_name_;
    std::string temperature_ = "294K";

    std::vector<double> evaluation_energies_;

    std::vector<float> total_xs_;
    std::vector<float> elastic_xs_;
    std::vector<float> inelastic_xs_;
    std::vector<float> capture_xs_;
    std::vector<float> fission_xs_;
};

} // namespace charmander
#endif  // CHARMANDER_MATERIALS_NUCLIDE_H_