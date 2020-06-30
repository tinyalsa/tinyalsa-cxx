cmake_minimum_required(VERSION 3.8.2)

function(add_tinyalsa_example example)

  set(target tinyalsa_example_${example})

  add_executable(${target} ${ARGN})

  target_link_libraries(${target} PRIVATE tinyalsa-cxx)

  target_compile_options(${target} PRIVATE ${tinyalsa_cxxflags})

  set_target_properties(${target}
    PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}"
      OUTPUT_NAME "${example}")

endfunction(add_tinyalsa_example example)

add_tinyalsa_example("input" "input.cpp")
