# --------------------------------------------------------------------------- #
# Setup code coverage
# --------------------------------------------------------------------------- #
option(BUILD_CHARMANDER_COVERAGE "Enable code coverage reporting" OFF)

if(BUILD_CHARMANDER_COVERAGE)
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(
      --coverage
      -O0
      -g
    )
    add_link_options(
      --coverage
    )
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(
      -O0
      -g
      -fprofile-instr-generate
      -fcoverage-mapping
    )
    add_link_options(
      -fprofile-instr-generate
    )
  else()
    message(FATAL_ERROR "Code coverage is only supported with GCC or Clang")
  endif()
endif()