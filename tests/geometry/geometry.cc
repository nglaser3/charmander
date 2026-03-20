#include <gtest/gtest.h>

#include <cmath>
#include <memory>

#include "env_wrapper.h"

#include "constants.h"
#include "basic_types.h"
#include "materials/nuclide.h"
#include "materials/ce_material.h"
#include "geometry/cylinder.h"
#include "geometry/cell.h"
#include "geometry/geometry.h"

namespace charmander {
  
class GeometryGeometry : public test_helpers::CharmanderXSEnvWrapper, public ::testing::Test {
 protected:
  std::shared_ptr<Cell> cell1_;
  std::shared_ptr<Cell> cell2_;
  std::shared_ptr<Nuclide> nuc_obj;
  // something goofy... idk segfaults otherwise if its defined in setup
  std::shared_ptr<ZCylinder> cyl1_ = std::make_shared<ZCylinder>(1.0, Point{0.0, 0.0, 0.0});
  std::shared_ptr<ZCylinder> cyl2_ = std::make_shared<ZCylinder>(2.0, Point{0.0, 0.0, 0.0});

  void SetUp() override {
    overwrite();
    const char* gotten = std::getenv(charmander_xs_.c_str());
    ASSERT_EQ(std::string(gotten), test_xs_dir_);

    nuc_obj = std::make_shared<Nuclide>(nuclide_);
    nuc_obj->LoadFromFile();
    CEMaterial mat(1, {{nuc_obj, 1.0}});

    Region region1({{-cyl1_}});
    Region region2 = +cyl1_ & -cyl2_;
    cell1_ = std::make_shared<Cell>(1, mat, region1);
    cell2_ = std::make_shared<Cell>(2, mat, region2);
  }

  void TearDown() override {
    reinstate();
  }
};

TEST_F(GeometryGeometry, Constructor) {
  EXPECT_NO_THROW(Geometry({}));
  EXPECT_NO_THROW(Geometry({*cell1_, *cell2_}));
}

TEST_F(GeometryGeometry, NewPosition) {
  double sqrt2 = std::sqrt(2);
  Geometry geom({*cell1_, *cell2_});

  Point in1 {0.0, 0.0, 0.0};
  Point on1 {sqrt2/2, sqrt2/2, 0.0};
  Point in2 {1.0, 1.0, 0.0};
  Point out {2.0, 2.0, 0.0};
  Point inf {INF, INF, INF};
  Direction xydir = normalize({1.0, 1.0, 0.0});

  // no direction
  EXPECT_EQ(geom.NewPosition(in1, xydir, 0.0, 1.0), in1);
  EXPECT_EQ(geom.NewPosition(in2, xydir, 0.0, 1.0), in2);

  // moving without leaving cell [mfp is 0.125 (totalxs is 8.0)]
  EXPECT_EQ(geom.NewPosition(in1, xydir, 1.0, 1.0), in1 + 1.0/8.0*xydir);

  // moving with leaving cell
  EXPECT_EQ(geom.NewPosition(in1, xydir, 12.0, 1.0), in1 + 1.5*xydir);

  // pass and travel out of bounds
  EXPECT_EQ(geom.NewPosition(in1, xydir, 20.0, 1.0), inf);

  // end on exactly surface
  EXPECT_EQ(geom.NewPosition(in1, xydir, 2*sqrt2, 1.0), in1 + 2*sqrt2/8.0 * xydir);

  // start exactly on surface
  EXPECT_EQ(geom.NewPosition(on1, xydir, 2.0, 1.0), on1 + 2.0/8.0*xydir);

  // start outside 
  EXPECT_EQ(geom.NewPosition(out, xydir, FP_TOLERANCE, 1.0), inf);
}

TEST_F(GeometryGeometry, CollisionType) {
  Geometry geom({*cell1_, *cell2_});

  Point in1 {0.0, 0.0, 0.0};

  EXPECT_EQ(geom.CollisionType(in1, 1.0, 0.5, FP_TOLERANCE).first, MT::ELASTIC);
  EXPECT_EQ(geom.CollisionType(in1, 1.0, 0.5, 0.25).first, MT::ELASTIC);

  EXPECT_EQ(geom.CollisionType(in1, 1.0, 0.5, 0.25 + FP_TOLERANCE).first, MT::INELASTIC);
  EXPECT_EQ(geom.CollisionType(in1, 1.0, 0.5 ,0.50).first, MT::INELASTIC);

  EXPECT_EQ(geom.CollisionType(in1, 1.0, 0.5 ,0.50 + FP_TOLERANCE).first, MT::FISSION);
  EXPECT_EQ(geom.CollisionType(in1, 1.0, 0.5, 0.75).first, MT::FISSION);

  EXPECT_EQ(geom.CollisionType(in1, 1.0, 0.5, 0.75 + FP_TOLERANCE).first, MT::CAPTURE);
  EXPECT_EQ(geom.CollisionType(in1, 1.0, 0.5, 1.0).first, MT::CAPTURE);

  EXPECT_EQ(geom.CollisionType(in1, 1.0, 0.5, 1.0 + FP_TOLERANCE).first, MT::MISSED);
}
}  // namespace charmander