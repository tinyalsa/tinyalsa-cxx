cmake_minimum_required(VERSION 2.8.11)

if (CMAKE_COMPILER_IS_GNUCXX)
	set (CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wfatal-errors -std=c++11")
endif (CMAKE_COMPILER_IS_GNUCXX)

add_library("tinyalsa-cxx"
	"config.cpp"
	"input-pcm.cpp"
	"pcm.cpp")
