cmake_minimum_required(VERSION 2.8.11)

add_custom_target("tinyalsa-cxx-headers" SOURCES
    "config.hpp"
    "input-pcm.hpp"
    "pcm.hpp")
