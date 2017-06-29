cmake_minimum_required(VERSION 2.8.11)

project("tinyalsa-cxx")

include_directories("include")

add_subdirectory("include/tinyalsa")
add_subdirectory("src")
add_subdirectory("examples")
