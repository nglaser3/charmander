#include "basic_types.h"

#include <gtest/gtest.h>

namespace charmander {

TEST(BasicTypes, PointConstructor) {
  Point p(0.0, 0.0, 0.0);
  EXPECT_DOUBLE_EQ(p.x, 0.0);
  EXPECT_DOUBLE_EQ(p.y, 0.0);
  EXPECT_DOUBLE_EQ(p.z, 0.0);
}

TEST(BasicTypes, DirectionConstructor) {
  Direction d(0.0, 0.0, 0.0);
  EXPECT_DOUBLE_EQ(d.x, 0.0);
  EXPECT_DOUBLE_EQ(d.y, 0.0);
  EXPECT_DOUBLE_EQ(d.z, 0.0);
}

TEST(BasicTypes, PointEquality) {
  Point a(1.0, 1.0, 1.0);
  Point b(1.0, 1.0, 1.0);
  Point c(1.0, 1.0, -1.0);
  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a == c);
}

TEST(BasicTypes, PointAddition) {
  Point a(1.0, 1.0, 1.0);
  Point b(1.0, 1.0, 1.0);

  EXPECT_EQ(a + b, Point(2.0, 2.0, 2.0));
}

TEST(BasicTypes, PointSubtraction) {
  Point a(1.0, 1.0, 1.0);
  Point b(1.0, 1.0, 1.0);

  EXPECT_EQ(a - b, Point(0.0, 0.0, 0.0));
}

TEST(BasicTypes, PointDirectionAddition) {
  Point p(1.0, 1.0, 1.0);
  Direction d(1.0, 1.0, 1.0);

  EXPECT_EQ(p + d, Point(2.0, 2.0, 2.0));
}

TEST(BasicTypes, PointDirectionSubtraction) {
  Point p(1.0, 1.0, 1.0);
  Direction d(1.0, 1.0, 1.0);

  EXPECT_EQ(p - d, Point(0.0, 0.0, 0.0));
}

TEST(BasicTypes, DirectionEquality) {
  Direction a(1.0, 1.0, 1.0);
  Direction b(1.0, 1.0, 1.0);
  Direction c(1.0, 1.0, -1.0);

  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a == c);
}

TEST(BasicTypes, DirectionAddition) {
  Direction a(1.0, 1.0, 1.0);
  Direction b(1.0, 1.0, 1.0);

  EXPECT_EQ(a + b, Direction(2.0, 2.0, 2.0));
}

TEST(BasicTypes, DirectionSubtraction) {
  Direction a(1.0, 1.0, 1.0);
  Direction b(1.0, 1.0, 1.0);

  EXPECT_EQ(a - b, Direction(0.0, 0.0, 0.0));
}

TEST(BasicTypes, DirectionNegation) {
  Direction d(1.0, 1.0, 1.0);

  EXPECT_EQ(-d, Direction(-1.0, -1.0, -1.0));
}

TEST(BasicTypes, DoubleDirectionMultiplication) {
  Direction d(1.0, 1.0, 1.0);

  EXPECT_EQ(3 * d, Direction(3.0, 3.0, 3.0));
}

}  // namespace charmander
