#include "env_wrapper.h"
#include "constants.h"
#include "materials/nuclide.h"

#include <gtest/gtest.h>

namespace charmander {

class MaterialsNuclide : public test_helpers::CharmanderXSEnvWrapper, public ::testing::Test {
 protected:
  void SetUp() override {
    overwrite();
    const char* gotten = std::getenv(charmander_xs_.c_str());
    ASSERT_EQ(std::string(gotten), test_xs_dir_);
  }

  void TearDown() override {
    reinstate();
  }
};

TEST_F(MaterialsNuclide, Constructor) { EXPECT_NO_THROW(Nuclide nuc(nuclide_)); }

TEST_F(MaterialsNuclide, LoadFromFile) {
  Nuclide nuc(nuclide_);
  EXPECT_FALSE(nuc.AlreadyLoaded());
  EXPECT_NO_THROW(nuc.LoadFromFile());
  EXPECT_TRUE(nuc.AlreadyLoaded());
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
