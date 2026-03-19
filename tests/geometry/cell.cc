#include <gtest/gtest.h>

#include <memory>

#include "env_wrapper.h"
#include "basic_types.h"
#include "constants.h"

#include "geometry/cell.h"
#include "geometry/cylinder.h"
#include "geometry/region.h"
#include "materials/ce_material.h"

namespace charmander
{
  
class GeometryCell : public test_helpers::CharmanderXSEnvWrapper, public ::testing::Test {
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

TEST_F(GeometryCell, Constructor) {
  NuclideData datum{nuc_obj_, 1.0};
  CEMaterial mat(1, {datum});
  auto cyl = XCylinder(1.0, {0.0, 0.0, 0.0});
  Region region({{-cyl}});

  Cell* cell;
  EXPECT_NO_THROW(cell = new Cell(1, mat, region));
  EXPECT_EQ(cell->GetUID(), 1);
}


TEST_F(GeometryCell, Contains) {
  NuclideData datum{nuc_obj_, 1.0};
  CEMaterial mat(1, {datum});
  auto cyl = XCylinder(1.0, {0.0, 0.0, 0.0});
  Region region({{-cyl}});

  Cell cell(1, mat, region);

  Point in{0.0, 0.0, 0.0};
  Point out{1.0, 1.0, 1.0};
  EXPECT_TRUE(cell.Contains(in));
  EXPECT_FALSE(cell.Contains(out));
}

TEST_F(GeometryCell, Distance) {
  NuclideData datum{nuc_obj_, 1.0};
  CEMaterial mat(1, {datum});
  auto cyl = XCylinder(1.0, {0.0, 0.0, 0.0});
  Region region({{-cyl}});

  Cell cell(1, mat, region);

  Point in{0.0, 0.0, 0.0};
  Point out{1.0, 1.0, 1.0}; 
  Direction dir{1.0, 0.0, 0.0};

  EXPECT_DOUBLE_EQ(cell.Distance(in, dir), region.Distance(in, dir));
  EXPECT_DOUBLE_EQ(cell.Distance(in, dir), cell.GetRegion().Distance(in, dir));
  EXPECT_DOUBLE_EQ(cell.Distance(out, dir), region.Distance(out, dir));
}

TEST_F(GeometryCell, TotalXS) {
  NuclideData datum{nuc_obj_, 1.0};
  CEMaterial mat(1, {datum});
  auto cyl = XCylinder(1.0, {0.0, 0.0, 0.0});
  Region region({{-cyl}});

  Cell cell(1, mat, region); 

  EXPECT_DOUBLE_EQ(cell.TotalXS(1.0), mat.GetTotalXS(1.0));
}

TEST_F(GeometryCell, Getters) {
  NuclideData datum{nuc_obj_, 1.0};
  CEMaterial mat(1, {datum});
  auto cyl = XCylinder(1.0, {0.0, 0.0, 0.0});
  Region region({{-cyl}});

  Cell cell(1, mat, region); 

  EXPECT_EQ(cell.GetUID(), 1);
  EXPECT_EQ(cell.GetFill().GetID(), mat.GetID());
}
} // namespace charmander