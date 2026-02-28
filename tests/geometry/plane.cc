#include "geometry/plane.h"

#include <gtest/gtest.h>

#include <iostream>

#include "constants.h"
#include "basic_types.h"

namespace charmander {
TEST(SurfacesPlane, PlaneConstructor) {
  EXPECT_NO_THROW(Plane(0.0, 0.0, 0.0, 0.0));
};

TEST(SurfacesPlane, PlaneSense) {
  Point pos(2.0, 2.0, 2.0);
  Point neg(0.0, 0.0, 0.0);

  XPlane xplane(1.0);
  EXPECT_TRUE(xplane.Sense(pos));
  EXPECT_FALSE(xplane.Sense(neg));

  YPlane yplane(1.0);
  EXPECT_TRUE(yplane.Sense(pos));
  EXPECT_FALSE(yplane.Sense(neg));

  ZPlane zplane(1.0);
  EXPECT_TRUE(zplane.Sense(pos));
  EXPECT_FALSE(zplane.Sense(neg));

  Plane allplane(1.0, 1.0, 1.0, 1.0);
  EXPECT_TRUE(allplane.Sense(pos));
  EXPECT_FALSE(allplane.Sense(neg));

  Plane close(0.0, 0.0, 0.0, 0.0);
  EXPECT_TRUE(close.Sense(neg));
};

TEST(SurfacesPlane, PlaneEvaluate) {
  Point p(5.0, 5.0, 5.0);

  XPlane xplane(1.0);
  EXPECT_DOUBLE_EQ(xplane.Evaluate(p), 4.0);

  YPlane yplane(1.0);
  EXPECT_DOUBLE_EQ(yplane.Evaluate(p), 4.0);

  ZPlane zplane(1.0);
  EXPECT_DOUBLE_EQ(zplane.Evaluate(p), 4.0);

  Plane allplane(1.0, 1.0, 1.0, 1.0);
  EXPECT_DOUBLE_EQ(allplane.Evaluate(p), 14.0);
};

TEST(SurfacesPlane, PlaneNormal) {
  Point p(0.0, 0.0, 0.0);

  EXPECT_EQ(XPlane(0.0).Normal(p), Direction(1.0, 0.0, 0.0));

  EXPECT_EQ(YPlane(0.0).Normal(p), Direction(0.0, 1.0, 0.0));

  EXPECT_EQ(ZPlane(0.0).Normal(p), Direction(0.0, 0.0, 1.0));

  EXPECT_EQ(Plane(1.0, 1.0, 1.0, 0.0).Normal(p), normalize({1.0, 1.0, 1.0}));
};

TEST(SurfacesPlane, PlaneDistance) {
  Point p(0.0, 0.0, 0.0);
  Direction all(1.0, 1.0, 1.0);

  Direction xperp_to(1.0, 0.0, 0.0);
  Direction xperp_away(-1.0, 0.0, 0.0);
  Direction xparallel(0.0, 1.0, 1.0);
  XPlane xplane(1.0);
  EXPECT_DOUBLE_EQ(xplane.Distance(p, xperp_to), 1.0);
  EXPECT_DOUBLE_EQ(xplane.Distance(p, xperp_away), INF);
  EXPECT_DOUBLE_EQ(xplane.Distance(p, xparallel), INF);

  Direction yperp_to(0.0, 1.0, 0.0);
  Direction yperp_away(0.0, -1.0, 0.0);
  Direction yparallel(1.0, 0.0, 1.0);
  YPlane yplane(1.0);
  EXPECT_DOUBLE_EQ(yplane.Distance(p, yperp_to), 1.0);
  EXPECT_DOUBLE_EQ(yplane.Distance(p, yperp_away), INF);
  EXPECT_DOUBLE_EQ(yplane.Distance(p, yparallel), INF);

  Direction zperp_to(0.0, 0.0, 1.0);
  Direction zperp_away(0.0, 0.0, -1.0);
  Direction zparallel(1.0, 1.0, 0.0);
  ZPlane zplane(1.0);
  EXPECT_DOUBLE_EQ(zplane.Distance(p, zperp_to), 1.0);
  EXPECT_DOUBLE_EQ(zplane.Distance(p, zperp_away), INF);
  EXPECT_DOUBLE_EQ(zplane.Distance(p, zparallel), INF);

  Direction perp_to(1.0, 1.0, 1.0);
  Direction perp_away(-1.0, -1.0, -1.0);
  Direction parallel(-1.0, 1.0, 0.0);
  Direction null(0.0, 0.0, 0.0);
  Plane plane(1.0, 1.0, 1.0, 1.0);
  EXPECT_DOUBLE_EQ(plane.Distance(p, perp_to), 1.0 / 3.0);
  EXPECT_DOUBLE_EQ(plane.Distance(p, perp_away), INF);
  EXPECT_DOUBLE_EQ(plane.Distance(p, parallel), INF);
  EXPECT_DOUBLE_EQ(plane.Distance(p, null), INF);
}
}  // namespace charmander