cmake_minimum_required(VERSION 3.8.2)

project("tinyalsa-cxx" CXX)

option(TINYALSA_EXAMPLES "Whether or not to build the examples." OFF)

set(common_warnings -Wall -Wextra -Werror -Wfatal-errors)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(tinyalsa_cxxflags ${common_warnings})
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(tinyalsa_cxxflags ${common_warnings} -Wdocumentation)
endif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")

add_library("tinyalsa-cxx"
  "tinyalsa.hpp"
  "tinyalsa.cpp")

target_compile_options("tinyalsa-cxx" PRIVATE ${tinyalsa_cxxflags})

target_include_directories("tinyalsa-cxx" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

if(TINYALSA_EXAMPLES)
  add_subdirectory("examples")
endif(TINYALSA_EXAMPLES)
