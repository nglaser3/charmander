#include "env_wrapper.h"
#include "constants.h"
#include "materials/ce_material.h"
#include "materials/nuclide.h"

#include <memory>
#include <vector>
#include <stdexcept>

#include <gtest/gtest.h>

namespace charmander {

class MaterialsCEMaterial : public test_helpers::CharmanderXSEnvWrapper, public ::testing::Test {
 protected:
  std::shared_ptr<Nuclide> nuc_obj_;
  void SetUp() override {
    overwrite();
    const char* gotten = std::getenv(charmander_xs_.c_str());
    ASSERT_EQ(std::string(gotten), test_xs_dir_);

    nuc_obj_ = std::make_shared<Nuclide>(nuclide_);
    nuc_obj_->LoadFromFile();
  }

  void TearDown() override {
    reinstate();
  }
};

TEST_F(MaterialsCEMaterial, CEMaterialConstructor) {
  NuclideData nucdatum_good{nuc_obj_, 1.0};
  CEMaterial* mat_good;
  EXPECT_NO_THROW(mat_good = new CEMaterial(1, {nucdatum_good}));
  EXPECT_EQ(mat_good->GetID(), 1);
  EXPECT_EQ(mat_good->GetNuclides().size(), 1);
  EXPECT_DOUBLE_EQ(mat_good->GetNuclides().at(0).atom_percent, 1.0);


  NuclideData nucdatum_renorm{nuc_obj_, 0.5};
  CEMaterial* mat_renorm;
  EXPECT_NO_THROW(mat_renorm = new CEMaterial(1, {nucdatum_renorm}));
  EXPECT_EQ(mat_renorm->GetNuclides().size(), 1);
  EXPECT_DOUBLE_EQ(mat_renorm->GetNuclides().at(0).atom_percent, 1.0);

  NuclideData nucdatum_neg{nuc_obj_, -1.0};
  EXPECT_THROW(CEMaterial(1, {nucdatum_neg}), std::runtime_error);

  std::vector<NuclideData> nucdata_none;
  EXPECT_THROW(CEMaterial(1, nucdata_none), std::runtime_error);
}

TEST_F(MaterialsCEMaterial, CEMaterialGetTotalXS) {
  NuclideData nucdatum1(nuc_obj_, 0.5);
  NuclideData nucdatum2(nuc_obj_, 0.5);
  CEMaterial mat(1, {nucdatum1, nucdatum2});
  ASSERT_FALSE(mat.GetNuclides().empty());
  for (const auto& nucdatum : mat.GetNuclides())
  {
    ASSERT_TRUE(nucdatum.nuc->AlreadyLoaded());
  }

  // energies are 0, 1, 2
  // total xs is 1, 2, 3
  // under clip
  EXPECT_DOUBLE_EQ(mat.GetTotalXS(-0.5), 1.0);
  EXPECT_DOUBLE_EQ(mat.GetTotalXS(0.0), 1.0);
  EXPECT_DOUBLE_EQ(mat.GetTotalXS(0.5), 1.5);
  // // over clip
  EXPECT_DOUBLE_EQ(mat.GetTotalXS(3.0), 3.0);
}

TEST_F(MaterialsCEMaterial, CEMaterialGetXSFromMT) {
 NuclideData nucdatum1(nuc_obj_, 0.5);
NuclideData nucdatum2(nuc_obj_, 0.5);
CEMaterial mat(1, {nucdatum1, nucdatum2});
ASSERT_FALSE(mat.GetNuclides().empty());
for (const auto& nucdatum : mat.GetNuclides())
{
  ASSERT_TRUE(nucdatum.nuc->AlreadyLoaded());
}

// energies are 0, 1, 2
// total xs is 1, 2, 3
// under clip
EXPECT_DOUBLE_EQ(mat.GetXSFromMT(MT::ELASTIC, -0.5), 1.0);
EXPECT_DOUBLE_EQ(mat.GetXSFromMT(MT::INELASTIC, 0.0), 1.0);
EXPECT_DOUBLE_EQ(mat.GetXSFromMT(MT::FISSION, 0.5), 1.5);
// // over clip
EXPECT_DOUBLE_EQ(mat.GetXSFromMT(MT::CAPTURE, 3.0), 3.0); 
}
}