#include "env_wrapper.h"
#include "constants.h"
#include "materials/ce_material.h"
#include "materials/nuclide.h"

#include <vector>
#include <stdexcept>

#include <gtest/gtest.h>

namespace charmander {

class MaterialsCEMaterial : public test_helpers::CharmanderXSEnvWrapper, public ::testing::Test {
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

TEST_F(MaterialsCEMaterial, CEMaterialConstructor) {
  NuclideData nucdatum_good{Nuclide(nuclide_), 1.0};
  CEMaterial* mat_good;
  EXPECT_NO_THROW(mat_good = new CEMaterial(1, {nucdatum_good}));
  EXPECT_EQ(mat_good->GetID(), 1);
  EXPECT_EQ(mat_good->GetNuclides().size(), 1);
  EXPECT_DOUBLE_EQ(mat_good->GetNuclides().at(0).atom_percent, 1.0);


  NuclideData nucdatum_renorm{Nuclide(nuclide_), 0.5};
  CEMaterial* mat_renorm;
  EXPECT_NO_THROW(mat_renorm = new CEMaterial(1, {nucdatum_renorm}));
  EXPECT_EQ(mat_renorm->GetNuclides().size(), 1);
  EXPECT_DOUBLE_EQ(mat_renorm->GetNuclides().at(0).atom_percent, 1.0);

  NuclideData nucdatum_neg{Nuclide(nuclide_), -1.0};
  EXPECT_THROW(CEMaterial(1, {nucdatum_neg}), std::runtime_error);
}
}