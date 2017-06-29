cmake_minimum_required(VERSION 2.8.11)

add_executable("input" "input.cpp")
target_link_libraries("input" "tinyalsa-cxx")
