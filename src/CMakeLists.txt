# --------------------------------------------------------------------------- #
# Configure Charmander hdf5 io library
# --------------------------------------------------------------------------- #
set(CHARMANDER_HDF5_XS_FILES
  materials/nuclide.cc
)

add_library(
  lib_charmander_xs ${CHARMANDER_HDF5_XS_FILES}
)
target_include_directories(
  lib_charmander_xs PUBLIC ${CHARMANDER_INCLUDE_DIR}
)
target_compile_features(
  lib_charmander_xs PUBLIC cxx_std_17
)
target_link_libraries(
  lib_charmander_xs PRIVATE ${HDF5_C_LIBRARIES} ${HDF5_C_HL_LIBRARIES}
)
target_include_directories(
  lib_charmander_xs PRIVATE ${HDF5_INCLUDE_DIRS}
)

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
target_link_libraries(
  lib_charmander PRIVATE lib_charmander_xs
)