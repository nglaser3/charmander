# --------------------------------------------------------------------------- #
# Configure Charmander library
# --------------------------------------------------------------------------- #
file(
  GLOB_RECURSE CHARMANDER_CC_FILES CONFIGURE_DEPENDS "${CHARMANDER_SRC_DIR}/*.cc"
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