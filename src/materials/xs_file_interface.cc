#include <cstdlib>
#include <vector>
#include <string>
#include <exception>
#include <filesystem>
#include <iostream>

#include <hdf5.h>
#include <hdf5_hl.h>

#include "materials/xs_file_interface.h"

namespace charmander
{
 XSFileInterface::XSFileInterface(const std::string& nuclide): nuclide_(nuclide) {
  std::string filepath = ResolveFilePath(nuclide);
  file_id_ = OpenXSFile(filepath);
 }

 XSFileInterface::~XSFileInterface() {
  CloseXSFile();
 }

 std::string XSFileInterface::ResolveFilePath(const std::string& nuclide) {
    // get xs env var, fail if not found
    const char* var = std::getenv("CHARMANDER_CROSS_SECTIONS");
    if (!var) {
      throw std::runtime_error("CHARMANDER_CROSS_SECTIONS is not set.");
    }

    // portable path (windows and unix)
    std::filesystem::path root_xs_dir(var);
    std::filesystem::path xs_file = root_xs_dir / (nuclide + ".h5");
    if (!std::filesystem::exists(xs_file)) {
      throw std::runtime_error("XS file for " + nuclide + " not found! Tried: " + xs_file.string());
    }

    return xs_file.string();
 }

 hid_t XSFileInterface::OpenXSFile(const std::string& filename) const {
  hid_t file_id = H5Fopen(filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
  if (file_id < 0) throw std::runtime_error("Failed to open HDF5 file: " + filename);
  return file_id;
 }

 void XSFileInterface::CloseXSFile() {
  if (file_id_ >= 0) H5Fclose(file_id_);
  file_id_ = -1; // avoids accidental attempts to reclose
 }

 void XSFileInterface::LoadEvaluationEnergies(const std::string& temperature, std::vector<double>& energies) const {
    std::string path = GetEnergyPath(temperature);
    size_t size = Get1DDatasetSize(path);
    energies.resize(size);
    if (H5LTread_dataset_double(file_id_, path.c_str(), energies.data()) < 0) {
      throw std::runtime_error("Failed to read evaluation energies: " + path);
    }
 }

 std::string XSFileInterface::GetEnergyPath(const std::string& temperature) const {
  return "/" + nuclide_ + "/energy/" + temperature; 
 }

 size_t XSFileInterface::Get1DDatasetSize(const std::string& dataset_path) const {
    hsize_t dims[1];
    H5T_class_t type_class;
    size_t type_size;

    if(H5LTget_dataset_info(file_id_, dataset_path.c_str(), dims, &type_class, &type_size) < 0) {
      throw std::runtime_error("Failed to get dataset info: " + dataset_path);
    }

    return static_cast<size_t>(dims[0]);
 } 

 void XSFileInterface::Load1DXSDataset(const std::string& mt_rxn, const std::string& temperature, std::vector<float>& xs, const size_t& target_size) const {
    std::string path = Get1DXSDataPath(mt_rxn, temperature);
    size_t size = Get1DDatasetSize(path);
    if (size != target_size) {
      throw std::runtime_error("XS data for MT " + mt_rxn + " has size " + std::to_string(size) + ", but energy size is " + std::to_string(target_size));
    }
    xs.resize(size);
    if (H5LTread_dataset_float(file_id_, path.c_str(), xs.data()) < 0) {
      throw std::runtime_error("Failed to read MT " + mt_rxn + " xs: " + path);
    }
 }

 void XSFileInterface::LeftPadLoad1DXSDataset(const std::string& mt_rxn, const std::string& temperature, std::vector<float>& xs, const size_t& target_size) const {
    std::string path = Get1DXSDataPath(mt_rxn, temperature);
    size_t size = Get1DDatasetSize(path);
    if (size > target_size) {
      throw std::runtime_error("Left pad specified for MT " + mt_rxn + " is larger than the corresponding energy grid.");
    }
    size_t left_pad_size = target_size - size;
    std::vector<float> temporary_holder(size); 
    if (H5LTread_dataset_float(file_id_, path.c_str(), temporary_holder.data()) < 0) {
      throw std::runtime_error("Failed to read MT " + mt_rxn + " xs: " + path);
    }
    xs.assign(target_size, 0.0f);
    std::copy(
      temporary_holder.begin(),
      temporary_holder.end(),
      xs.begin() + left_pad_size
    );
 }

 std::string XSFileInterface::Get1DXSDataPath(const std::string& mt_rxn, const std::string& temperature) const {
  return "/" + nuclide_ + "/reactions/reaction_" + mt_rxn + "/" + temperature + "/xs";
 }
} // namespace charmander