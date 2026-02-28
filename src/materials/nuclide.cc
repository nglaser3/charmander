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
  std::string resolve_xs_file_path(const std::string& nuclide_name) {
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

    return xs_file.string();
  };

  hid_t open_xs_file(const std::string& filename) {
    hid_t file_id = H5Fopen(filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    if (file_id < 0) throw std::runtime_error("Failed to open HDF5 file: " + filename);
    return file_id;
  };

  std::string get_energy_path(const std::string& nuclide_name, const std::string& temperature) {
    return "/" + nuclide_name + "/energy/" + temperature; 
  }

  std::string get_xs_1D_data_path(const std::string& nuclide_name, const std::string& mt_number, const std::string& temperature) {
    return "/" + nuclide_name + "/reactions/reaction_" + mt_number + "/" + temperature + "/xs";
  }

  // ONLY WORKS FOR 1D DATASETS
  size_t get_1D_dataset_size(hid_t file_id, const std::string& dataset_path) {
    hsize_t dims[1];
    H5T_class_t type_class;
    size_t type_size;

    if(H5LTget_dataset_info(file_id, dataset_path.c_str(), dims, &type_class, &type_size) < 0) {
      throw std::runtime_error("Failed to get dataset info: " + dataset_path);
    }

    return static_cast<size_t>(dims[0]);
  }
  
  void load_in_eval_energies(hid_t file_id, const std::string& nuclide_name, const std::string& temperature, std::vector<double>& energies) {
    std::string path = hdf5_helpers::get_energy_path(nuclide_name, temperature);
    size_t size = hdf5_helpers::get_1D_dataset_size(file_id, path);
    energies.resize(size);
    if (H5LTread_dataset_double(file_id, path.c_str(), energies.data()) < 0) {
      throw std::runtime_error("Failed to read evaluation energies: " + path);
    }
  }

  void attempt_load_in_xs(hid_t file_id, const std::string& nuclide_name, const std::string& temperature, std::vector<float>& total_xs, const std::string& reaction, std::vector<float>& xs) {
    std::string path = hdf5_helpers::get_xs_1D_data_path(nuclide_name, reaction, temperature);
    if (H5LTpath_valid(file_id, path.c_str(), 1) <= 0) {xs.clear(); return;}
    xs.resize(total_xs.size());
    if (H5LTread_dataset_float(file_id, path.c_str(), xs.data()) < 0) {
      throw std::runtime_error("Failed to read MT " + reaction + " xs: " + path);
    }
    for (size_t idx=0; idx<total_xs.size(); idx++)
    {
      total_xs[idx] += xs[idx];
    }
  }

} // namespace hdf5_helpers 

namespace charmander
{
  
void Nuclide::LoadFromFile() {

  // find and open xs file
  std::string xs_file = hdf5_helpers::resolve_xs_file_path(nuclide_name_);
  hid_t file_id = hdf5_helpers::open_xs_file(xs_file);

  // energies 
  hdf5_helpers::load_in_eval_energies(file_id, nuclide_name_, temperature_, evaluation_energies_);

  // lambda for no repeat arguments
  auto fill_xs = [&](const char* mt, std::vector<float>& xs) {
    hdf5_helpers::attempt_load_in_xs(file_id, nuclide_name_, temperature_, total_xs_, mt, xs);
  };

  // fill the xs
  total_xs_.assign(evaluation_energies_.size(), 0.0f);
  fill_xs("002", elastic_xs_);
  fill_xs("004", inelastic_xs_);
  fill_xs("018", fission_xs_);
  fill_xs("102", capture_xs_);

  // close file
  // DO NOT FORGET TO DO THIS, MEMORY LEAK OTHERWISE
  H5Fclose(file_id);
};

} // namespace charmander
