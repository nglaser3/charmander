# --------------------------------------------------------------------------- #
# Yoink Google test
# --------------------------------------------------------------------------- #
include(FetchContent)

set(
  BUILD_GMOCK
  OFF
  CACHE BOOL "" FORCE
)
set(
  INSTALL_GTEST
  OFF
  CACHE BOOL "" FORCE
)
set(
  gtest_force_shared_crt
  ON 
  CACHE BOOL "" FORCE
)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0 
)
FetchContent_MakeAvailable(googletest)

# --------------------------------------------------------------------------- #
# Setup test executable
# --------------------------------------------------------------------------- #
enable_testing()
include(GoogleTest)

file(
  GLOB_RECURSE TEST_CC_FILES CONFIGURE_DEPENDS 
  "${CHARMANDER_TEST_DIR}/*.cc"
)
add_executable(
  charmander_tests ${TEST_CC_FILES}
)
target_include_directories(
  charmander_tests PRIVATE ${CHARMANDER_INCLUDE_DIR}
)
target_link_libraries(
  charmander_tests PRIVATE GTest::gtest_main lib_charmander
)
gtest_discover_tests(charmander_tests)