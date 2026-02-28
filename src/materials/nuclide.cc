#include <cstdlib>
#include <vector>
#include <string>
#include <exception>
#include <filesystem>
#include <iostream>

#include <hdf5.h>
#include <hdf5_hl.h>

#include "materials/nuclide.h"

namespace hdf5_helpers
{
  std::filesystem::path resolve_xs_file_path(std::string nuclide_name) {
    // get xs env var, fail if not found
    const char* var = std::getenv("CHARMANDER_CROSS_SECTIONS");
    if (!var) {
      throw std::runtime_error("CHARMANDER_CROSS_SECTIONS is not set.");
    }

    // portable path (windows vs unix)
    std::filesystem::path root_xs_dir(var);
    std::filesystem::path xs_file = root_xs_dir / (nuclide_name + ".h5");
    if (!std::filesystem::exists(xs_file)) {
      throw std::runtime_error("XS file for " + nuclide_name + " not found! " + "Tried: " + xs_file.string());
    }

    return xs_file;
  };

  hid_t open_xs_file(std::filesystem::path filename) {
    hid_t file_id = H5Fopen(filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    if (file_id < 0) throw std::runtime_error("Failed to open HDF5 file: " + filename.string());
    return file_id;
  };

  bool does_attribute_exist(hid_t file_id, std::string attribute)
  {
    return 0 < H5Aexists_by_name(file_id, ".", attribute.c_str(), H5P_DEFAULT);
  };

  std::string get_energy_path(std::string nuclide_name, std::string temperature) {
    return "/" + nuclide_name + "/energy/" + temperature; 
  }

  std::string get_xs_1D_data_path(std::string nuclide_name, std::string mt_number, std::string temperature) {
    return "/" + nuclide_name + "/reactions/reaction_" + mt_number + "/" + temperature + "/xs";
  }

  // ONLY WORKS FOR 1D DATASETS
  size_t get_dataset_size(hid_t file_id, const std::string dataset_path) {
    hsize_t dims[1];
    H5T_class_t type_class;
    size_t type_size;

    if(H5LTget_dataset_info(file_id, dataset_path.c_str(), dims, &type_class, &type_size) < 0) {
      throw std::runtime_error("Failed to get dataset info: " + dataset_path);
    }

    return static_cast<size_t>(dims[0]);
  }

} // namespace hdf5_helpers 

namespace charmander
{
  
void Nuclide::LoadFromFile() {

  std::filesystem::path xs_file = hdf5_helpers::resolve_xs_file_path(nuclide_name_);
  hid_t file_id = hdf5_helpers::open_xs_file(xs_file);

  // energies 
  std::string energy_path = hdf5_helpers::get_energy_path(nuclide_name_, temperature_);
  size_t energy_size = hdf5_helpers::get_dataset_size(file_id, energy_path);
  evaluation_energies_.resize(energy_size);
  if (H5LTread_dataset_double(file_id, energy_path.c_str(), evaluation_energies_.data()) < 0) {
    throw std::runtime_error("Failed to read evaluation energies: " + energy_path);
  }

  // elastic xs
  std::string elastic_path = hdf5_helpers::get_xs_1D_data_path(nuclide_name_, "002", temperature_);
  elastic_xs_.resize(energy_size);
  if (H5LTread_dataset_float(file_id, elastic_path.c_str(), elastic_xs_.data()) < 0) {
    throw std::runtime_error("Failed to read elastic scattering xs: " + elastic_path);
  }

  // inelastic xs
  
  // capture xs
  //std::string capture_xs = hdf5_helpers::get_xs_1D_data_path(nuclide_name_, "102", temperature_)

};

} // namespace charmander
