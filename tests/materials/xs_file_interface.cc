#include <gtest/gtest.h>

#include "materials/xs_file_interface.h"

namespace charmander
{
  TEST(MaterialsXSFileInterface, Constructor) {
    EXPECT_NO_THROW(XSFileInterface("U235"));
  }
} // namespace charmander
