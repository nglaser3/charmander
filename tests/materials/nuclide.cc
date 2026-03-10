#include <filesystem>

#include "constants.h"
#include "materials/nuclide.h"

#include <gtest/gtest.h>

namespace charmander {

class MaterialsNuclide : public ::testing::Test {
 protected:
  std::string test_xs_dir_;
  bool has_original_{false};
  std::string old_env_var_;
  std::string nuclide_{"FakeU235"};
  std::string charmander_xs_{"CHARMANDER_CROSS_SECTIONS"};

  void SetUp() override {
    const char* var = std::getenv(charmander_xs_.c_str());
    if (var) {
      has_original_ = true;
      old_env_var_ = var;
    }

    // defined in test level cmake, avoids having to determine path at runtime
    std::filesystem::path test_xs_root = CHARMANDER_TEST_DATA_DIR;
    test_xs_dir_ = (test_xs_root / "fake_cross_sections").string();

#ifdef _WIN32
    _putenv_s(charmander_xs_.c_str(), test_xs_dir_.c_str());
#else
    setenv(charmander_xs_.c_str(), test_xs_dir_.c_str(), 1);
#endif

    const char* gotten = std::getenv(charmander_xs_.c_str());
    ASSERT_EQ(std::string(gotten), test_xs_dir_);
  }

  void TearDown() override {
    if (has_original_) {
#ifdef _WIN32
      _putenv_s(charmander_xs_.c_str(), old_env_var_.c_str());
#else
      setenv(charmander_xs_.c_str(), old_env_var_.c_str(), 1);
#endif
    } else {
#ifdef _WIN32
      _putenv((charmander_xs_ + "=").c_str());
#else
      unsetenv(charmander_xs_.c_str());
#endif
    }
  }
};

TEST_F(MaterialsNuclide, Constructor) { EXPECT_NO_THROW(Nuclide nuc(nuclide_)); }

TEST_F(MaterialsNuclide, LoadFromFile) {
  Nuclide nuc(nuclide_);
  EXPECT_NO_THROW(nuc.LoadFromFile());
}

TEST_F(MaterialsNuclide, GetLowerEnergyBin) {
  Nuclide nuc(nuclide_);
  nuc.LoadFromFile();

  // energies are 0, 1, 2
  EXPECT_EQ(nuc.GetLowerEnergyBin(0.0 - FP_TOLERANCE), 0);
  EXPECT_EQ(nuc.GetLowerEnergyBin(0.0), 0);
  EXPECT_EQ(nuc.GetLowerEnergyBin(0.0 + FP_TOLERANCE), 0);
  EXPECT_EQ(nuc.GetLowerEnergyBin(2.0 - FP_TOLERANCE), 1);
  // Expect 1 because we clip
  EXPECT_EQ(nuc.GetLowerEnergyBin(2.0 + FP_TOLERANCE), 1);
}

TEST_F(MaterialsNuclide, GetTotalXS) {
  Nuclide nuc(nuclide_);
  nuc.LoadFromFile();

  // energies are 0, 1, 2
  // total xs is 1, 2, 3

  // expect 1.0 (min) because we clip at boundaries
  size_t under_energy = nuc.GetLowerEnergyBin(-1.0);
  EXPECT_DOUBLE_EQ(nuc.GetTotalXS(under_energy, -1.0), 1.0);

  // expect 1.5 from linear interpolation
  size_t norm_energy = nuc.GetLowerEnergyBin(0.5);
  EXPECT_DOUBLE_EQ(nuc.GetTotalXS(norm_energy, 0.5), 1.5);

  // expect 3.0 (max) because we clip at boundaries
  size_t over_energy = nuc.GetLowerEnergyBin(2.5);
  EXPECT_DOUBLE_EQ(nuc.GetTotalXS(over_energy, 2.5), 3.0);
}

TEST_F(MaterialsNuclide, GetXSFromMT) {
  Nuclide nuc(nuclide_);
  nuc.LoadFromFile();

  // energies are 0, 1, 2
  // all xs is 1, 2, 3

  // expect 1.0 (min) because we clip at boundaries
  size_t under_energy = nuc.GetLowerEnergyBin(-1.0);
  EXPECT_DOUBLE_EQ(nuc.GetXSFromMT(MT::ELASTIC, under_energy, -1.0), 1.0);
  EXPECT_DOUBLE_EQ(nuc.GetXSFromMT(MT::INELASTIC, under_energy, -1.0), 1.0);
  EXPECT_DOUBLE_EQ(nuc.GetXSFromMT(MT::FISSION, under_energy, -1.0), 1.0);
  EXPECT_DOUBLE_EQ(nuc.GetXSFromMT(MT::CAPTURE, under_energy, -1.0), 1.0);

  // expect 1.5 from linear interpolation
  size_t norm_energy = nuc.GetLowerEnergyBin(0.5);
  EXPECT_DOUBLE_EQ(nuc.GetXSFromMT(MT::ELASTIC, norm_energy, 0.5), 1.5);

  // expect 3.0 (max) because we clip at boundaries
  size_t over_energy = nuc.GetLowerEnergyBin(2.5);
  EXPECT_DOUBLE_EQ(nuc.GetXSFromMT(MT::ELASTIC, over_energy, 2.5), 3.0);
}
}  // namespace charmander
