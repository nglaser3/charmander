#include "geometry/cylinder.h"

#include <gtest/gtest.h>

#include <cmath>

#include "constants.h"
#include "basic_types.h"

namespace charmander {
TEST(SurfacesCylinder, CylinderConstructor) {
  Point p0(0.0, 0.0, 0.0);
  Point p1(1.0, 2.0, 3.0);
  Direction d(1.0, 0.0, 0.0);

  EXPECT_NO_THROW(XCylinder(1.0, p0));
  EXPECT_NO_THROW(XCylinder(1.0, p1));
  EXPECT_NO_THROW(YCylinder(1.0, p0));
  EXPECT_NO_THROW(YCylinder(1.0, p1));
  EXPECT_NO_THROW(ZCylinder(1.0, p0));
  EXPECT_NO_THROW(ZCylinder(1.0, p1));
  EXPECT_NO_THROW(Cylinder(1.0, d, p0));
};

TEST(SurfacesCylinder, CylinderSense) {
  Point c(0.0, 0.0, 0.0);
  Point neg(0.5, 0.5, 0.5);
  Point pos(5.0, 5.0, 0.0);

  XCylinder xcyl(1.0, c);
  EXPECT_TRUE(xcyl.Sense(pos));
  EXPECT_FALSE(xcyl.Sense(neg));

  YCylinder ycyl(1.0, c);
  EXPECT_TRUE(ycyl.Sense(pos));
  EXPECT_FALSE(ycyl.Sense(neg));

  ZCylinder zcyl(1.0, c);
  EXPECT_TRUE(zcyl.Sense(pos));
  EXPECT_FALSE(zcyl.Sense(neg));

  Direction d(1.0, 1.0, 1.0);
  Cylinder cyl(1.0, d, c);
  EXPECT_TRUE(cyl.Sense(pos));
  EXPECT_FALSE(cyl.Sense(neg));
};

TEST(SurfacesCylinder, CylinderEvaluate) {
  double sqrt2 = std::sqrt(2);

  Point c(0.0, 0.0, 0.0);

  Point p_on(1/sqrt2, 1/sqrt2, 1/sqrt2);
  Point p_off(1.0, 1.0, 1.0);

  XCylinder xcyl(1.0, c);
  EXPECT_NEAR(xcyl.Evaluate(p_on), 0.0, COINCIDENT_SURF);
  EXPECT_NEAR(xcyl.Evaluate(p_on + Point(100.0, 0.0, 0.0)), 0.0, COINCIDENT_SURF);
  EXPECT_NEAR(xcyl.Evaluate(p_off), 1.0, COINCIDENT_SURF);

  YCylinder ycyl(1.0, c);
  EXPECT_NEAR(ycyl.Evaluate(p_on), 0.0, COINCIDENT_SURF);
  EXPECT_NEAR(ycyl.Evaluate(p_on + Point(0.0, 100.0, 0.0)), 0.0, COINCIDENT_SURF);
  EXPECT_NEAR(ycyl.Evaluate(p_off), 1.0, COINCIDENT_SURF);

  ZCylinder zcyl(1.0, c);
  EXPECT_NEAR(zcyl.Evaluate(p_on), 0.0, COINCIDENT_SURF);
  EXPECT_NEAR(zcyl.Evaluate(p_on + Point(0.0, 0.0, 100.0)), 0.0, COINCIDENT_SURF);
  EXPECT_NEAR(zcyl.Evaluate(p_off), 1.0, COINCIDENT_SURF);

  Direction axis = normalize({1.0, 1.0, 1.0});
  Direction perp = normalize({1.0, -1.0, 0.0});
  Point p1_on = c + perp;
  Point p2_on = p1_on + 100.0 * axis;
  Point pc_off = p1_on + perp;
  Cylinder cyl(1.0, axis, c);
  EXPECT_NEAR(cyl.Evaluate(p1_on), 0.0, COINCIDENT_SURF);
  EXPECT_NEAR(cyl.Evaluate(p2_on), 0.0, COINCIDENT_SURF);
  EXPECT_NEAR(cyl.Evaluate(pc_off), 3.0, COINCIDENT_SURF);
};

TEST(SurfacesCylinder, CylinderNormal) {
  double sqrt2 = std::sqrt(2.0);
  double sqrt3 = std::sqrt(3.0);

  Point px1(0.0, 1/sqrt2, 1/sqrt2);
  Point px2(100.0, 1/sqrt2, 1/sqrt2);
  Direction dx(normalize({0.0, 1.0, 1.0}));
  Point cx(0.0, 0.0, 0.0);
  XCylinder xcyl(1.0, cx);
  EXPECT_TRUE(fuzzyequal(xcyl.Normal(px1), dx));
  EXPECT_TRUE(fuzzyequal(xcyl.Normal(px2), dx));

  Point py1(1/sqrt2, 0.0, 1/sqrt2);
  Point py2(1/sqrt2, 100.0, 1/sqrt2);
  Direction dy(normalize({1.0, 0.0, 1.0}));
  Point cy(0.0, 0.0, 0.0);
  YCylinder ycyl(1.0, cy);
  EXPECT_TRUE(fuzzyequal(ycyl.Normal(py1), dy));
  EXPECT_TRUE(fuzzyequal(ycyl.Normal(py2), dy));

  Point pz1(1/sqrt2, 1/sqrt2, 0.0);
  Point pz2(1/sqrt2, 1/sqrt2, 100.0);
  Direction dz(normalize({1.0, 1.0, 0.0}));
  Point cz(0.0, 0.0, 0.0);
  ZCylinder zcyl(1.0, cz);
  EXPECT_TRUE(fuzzyequal(zcyl.Normal(pz1), dz));
  EXPECT_TRUE(fuzzyequal(zcyl.Normal(pz2), dz));

  Direction axis = normalize({1.0, 1.0, 1.0});
  Direction perp = normalize({1.0, -1.0, 0.0});
  Point c(0.0, 0.0, 0.0);
  Cylinder cyl(1.0, axis, c);
  Point p1 = c + perp;
  Point p2 = p1 + 100.0 * axis;
  EXPECT_TRUE(fuzzyequal(cyl.Normal(p1), perp));
  EXPECT_TRUE(fuzzyequal(cyl.Normal(p2), perp));
};

TEST(SurfacesCylinder, CylinderDistance) {
  Point c(0.0, 0.0, 0.0);

  Point px(0.0, 0.0, 2.0);
  Direction xperpto(0.0, 0.0, -1.0);
  Direction xperpaway = -xperpto;
  Direction xparallel(1.0, 0.0, 0.0);
  XCylinder xcyl(1.0, c);
  EXPECT_DOUBLE_EQ(xcyl.Distance(px, xperpto), 1.0);
  EXPECT_DOUBLE_EQ(xcyl.Distance(px, xperpaway), INF);
  EXPECT_DOUBLE_EQ(xcyl.Distance(px, xparallel), INF);

  Point py(2.0, 0.0, 0.0);
  Direction yperpto(-1.0, 0.0, 0.0);
  Direction yperpaway = -yperpto;
  Direction yparallel(0.0, 1.0, 0.0);
  YCylinder ycyl(1.0, c);
  EXPECT_DOUBLE_EQ(ycyl.Distance(py, yperpto), 1.0);
  EXPECT_DOUBLE_EQ(ycyl.Distance(py, yperpaway), INF);
  EXPECT_DOUBLE_EQ(ycyl.Distance(py, yparallel), INF);

  Point pz(0.0, 2.0, 0.0);
  Direction zperpto(0.0, -1.0, 0.0);
  Direction zperpaway = -zperpto;
  Direction zparallel(0.0, 0.0, 1.0);
  ZCylinder zcyl(1.0, c);
  EXPECT_DOUBLE_EQ(zcyl.Distance(pz, zperpto), 1.0);
  EXPECT_DOUBLE_EQ(zcyl.Distance(pz, zperpaway), INF);
  EXPECT_DOUBLE_EQ(zcyl.Distance(pz, zparallel), INF);

  Direction axis = normalize({1.0, 1.0, 1.0});
  Direction vhat = normalize({1.0, -1.0, 0.0});
  Point p(2.0 * vhat.x, 2.0 * vhat.y, 2.0 * vhat.z);
  Direction perpto = -vhat;
  Direction perpaway = -perpto;
  Direction parallel = axis;
  Direction no_intersect = normalize({1.0, 1.0, 0.0});
  Cylinder cyl(1.0, axis, c);
  EXPECT_DOUBLE_EQ(cyl.Distance(p, perpto), 1.0);
  EXPECT_DOUBLE_EQ(cyl.Distance(p, perpaway), INF);
  EXPECT_DOUBLE_EQ(cyl.Distance(p, parallel), INF);
  EXPECT_DOUBLE_EQ(cyl.Distance(p, no_intersect), INF);
}
}  // namespace charmander
