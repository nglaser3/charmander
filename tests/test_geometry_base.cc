#include <gtest/gtest.h>

#include "geometry_base.h"

namespace charmander {

TEST(GeometryBase, BasicConstructor) { EXPECT_NO_THROW(GeometryBase()); }

}  // namespace charmander