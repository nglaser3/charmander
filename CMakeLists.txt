# --------------------------------------------------------------------------- #
# Project configuration 
# --------------------------------------------------------------------------- #
cmake_minimum_required(VERSION 3.24)

project(
  charmander
  VERSION 0.0.0
)

# --------------------------------------------------------------------------- #
# Load custom CMake modules
# --------------------------------------------------------------------------- #
list(
  APPEND CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
)

# path configurations
include(CharmanderPaths)

# --------------------------------------------------------------------------- #
# Source
# --------------------------------------------------------------------------- #

add_subdirectory(src)

# --------------------------------------------------------------------------- #
# Testing
# --------------------------------------------------------------------------- #
option(CHARMANDER_BUILD_TESTS "Build charmander test suite" OFF)
if(CHARMANDER_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
