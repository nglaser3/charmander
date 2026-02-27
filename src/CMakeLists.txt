# --------------------------------------------------------------------------- #
# Configure main Charmander library
# --------------------------------------------------------------------------- #
set(CHARMANDER_CC_FILES
  geometry/geometry.cc
  geometry/surface.cc
  geometry/cylinder.cc
  geometry/plane.cc
)

add_library(
  lib_charmander ${CHARMANDER_CC_FILES}
)
target_include_directories(
  lib_charmander PUBLIC ${CHARMANDER_INCLUDE_DIR}
)
target_compile_features(
  lib_charmander PUBLIC cxx_std_17
)