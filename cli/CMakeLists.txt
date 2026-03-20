# --------------------------------------------------------------------------- #
# Configure Charmander run executable
# --------------------------------------------------------------------------- #
set(CHARMANDER_RUN_FILES
  main.cc
)
add_executable(
  run_charmander ${CHARMANDER_RUN_FILES}
)
target_include_directories(
  run_charmander PRIVATE ${CHARMANDER_CLI_DIR}
)
target_compile_features(
  run_charmander PRIVATE cxx_std_20
)
target_link_libraries(
  run_charmander PRIVATE lib_charmander
)
set_target_properties(run_charmander PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)