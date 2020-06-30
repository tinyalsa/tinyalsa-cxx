cmake_minimum_required(VERSION 2.8.11)

add_library("tinyalsa-cxx"
  "pcm.cpp"
  "interleaved_reader.cpp")

target_include_directories("tinyalsa-cxx" PUBLIC "${PROJECT_SOURCE_DIR}/include")

target_compile_options("tinyalsa-cxx" PRIVATE ${tinyalsa_cxxflags})
