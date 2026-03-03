#ifndef CHARMANDER_MATERIALS_NUCLIDE_H_
#define CHARMANDER_MATERIALS_NUCLIDE_H_

#include <string>
#include <unordered_map>
#include <vector>

namespace charmander {
enum MT {
  ELASTIC = 2,
  INELASTIC = 4,
  FISSION = 18,
  CAPTURE = 102,
};

class Nuclide {
 public:
  Nuclide(std::string nuclide) : nuclide_name_(nuclide){};

  void LoadFromFile();

  void ConstructTotalXS(std::vector<float> total_xs) const;

  size_t GetLowerEnergyBin(double energy) const;

  double GetTotalXS(size_t energy_index, double energy) const;

  double GetXSFromMT(MT mt, size_t energy_index, double energy) const;

 private:
  std::string nuclide_name_;
  std::string temperature_ = "294K";

  std::vector<double> evaluation_energies_;

  std::vector<float> total_xs_;
  std::unordered_map<MT, std::vector<float>> xs_map_;
};

}  // namespace charmander
#endif  // CHARMANDER_MATERIALS_NUCLIDE_H_