#include "basic_types.h"

#include <gtest/gtest.h>

#include <cmath>
#include <iostream>

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

TEST(BasicTypes, DirectionConstructorFromPoint) {
  Point p(1.0, 1.0, 1.0);
  Direction d(p);

  EXPECT_EQ(d, Direction(1.0, 1.0, 1.0));
}

TEST(BasicTypes, PointEquality) {
  Point a(1.0, 0.0, 0.0);
  Point b(0.0, 1.0, 0.0);
  Point c(0.0, 0.0, 1.0);
  Point d(0.0, 0.0, 0.0);
  Point e(0.0, 0.0, 0.0);
  EXPECT_FALSE(a == b);
  EXPECT_FALSE(b == c);
  EXPECT_FALSE(c == d);
  EXPECT_TRUE(d == e);
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

TEST(BasicTypes, PointOutputStream) {
  Point d(1.0, 2.0, 3.0);

  std::ostringstream outputstream;
  outputstream << d;
  EXPECT_EQ(outputstream.str(), "Point(1, 2, 3)");
}

TEST(BasicTypes, FuzzyEqualPoint) {
  Point a(1.0, 0.0, 0.0);
  Point b(0.0, 1.0, 0.0);
  Point c(0.0, 0.0, 1.0);
  Point d(0.0, 0.0, 0.0);
  Point e(0.0, 0.0, 0.0);
  EXPECT_FALSE(fuzzyequal(a, b));
  EXPECT_FALSE(fuzzyequal(b, c));
  EXPECT_FALSE(fuzzyequal(c, d));
  EXPECT_TRUE(fuzzyequal(d, e));
}

TEST(BasicTypes, DirectionEquality) {
  Direction a(1.0, 0.0, 0.0);
  Direction b(0.0, 1.0, 0.0);
  Direction c(0.0, 0.0, 1.0);
  Direction d(0.0, 0.0, 0.0);
  Direction e(0.0, 0.0, 0.0);
  EXPECT_FALSE(a == b);
  EXPECT_FALSE(b == c);
  EXPECT_FALSE(c == d);
  EXPECT_TRUE(d == e);
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

TEST(BasicTypes, DirectionDirectionMultiplication) {
  Direction a(1.0, 1.0, 1.0);
  Direction b(3.0, 4.0, 5.0);

  EXPECT_DOUBLE_EQ(a * b, 12.0);
}

TEST(BasicTypes, DirectionOutputStream) {
  Direction d(1.0, 2.0, 3.0);

  std::ostringstream outputstream;
  outputstream << d;
  EXPECT_EQ(outputstream.str(), "Direction(1, 2, 3)");
}

TEST(BasicTypes, NormalizeDirection) {
  Direction d(1.0, 1.0, 1.0);

  EXPECT_EQ(normalize(d),
            Direction(1 / std::sqrt(3), 1 / std::sqrt(3), 1 / std::sqrt(3)));
}

TEST(BasicTypes, FuzzyEqualDirection) {
  Direction a(1.0, 0.0, 0.0);
  Direction b(0.0, 1.0, 0.0);
  Direction c(0.0, 0.0, 1.0);
  Direction d(0.0, 0.0, 0.0);
  Direction e(0.0, 0.0, 0.0);
  EXPECT_FALSE(fuzzyequal(a, b));
  EXPECT_FALSE(fuzzyequal(b, c));
  EXPECT_FALSE(fuzzyequal(c, d));
  EXPECT_TRUE(fuzzyequal(d, e));
}

}  // namespace charmander

