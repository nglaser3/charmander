#include <gtest/gtest.h>

#include "geometry/geometry.h"

namespace charmander {

TEST(Geometry, BasicConstructor) { EXPECT_NO_THROW(Geometry()); }

}  // namespace charmander