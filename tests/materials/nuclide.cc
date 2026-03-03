#include "materials/nuclide.h"

#include <gtest/gtest.h>

namespace charmander {
TEST(MaterialsNuclide, Constructor) { EXPECT_NO_THROW(Nuclide("U235")); }

TEST(MaterialsNuclide, LoadFromFile) {
  Nuclide nuc("U235");
  EXPECT_NO_THROW(nuc.LoadFromFile());
}
}  // namespace charmander
