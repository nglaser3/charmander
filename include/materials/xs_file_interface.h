#ifndef CHARMANDER_MATERIALS_XS_FILE_INTERFACE_H_
#define CHARMANDER_MATERIALS_XS_FILE_INTERFACE_H_

#include <string>
#include <vector>

#include <hdf5.h>

namespace charmander
{
  class XSFileInterface
  {
  public:
    XSFileInterface(const std::string& nuclide);
    ~XSFileInterface();

    std::string ResolveFilePath(const std::string& nuclide) const;

    hid_t OpenXSFile(const std::string& filename) const;

    void CloseXSFile();

    void LoadEvaluationEnergies(const std::string& temperature, std::vector<double>& energies) const;

    std::string GetEnergyPath(const std::string& temperature) const;

    size_t Get1DDatasetSize(const std::string& dataset_path) const;

    void Load1DXSDataset(const std::string& mt_rxn, const std::string& temperature, std::vector<float>& xs, const size_t& target_size) const;

    std::string Get1DXSDataPath(const std::string& mt_rxn, const std::string& temperature) const;

  private:
    const std::string nuclide_;
    hid_t file_id_;
  };
  
} // namespace charmander


#endif  // CHARMANDER_MATERIALS_XS_FILE_INTERFACE_H_