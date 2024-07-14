cmake_minimum_required(VERSION 2.8)

project(cppMazeGen CXX C)

add_compile_options(--std=c++11 -Wall)

include_directories(src)

file(GLOB_RECURSE srcs src/*.cpp)
add_executable(cppMazeGen ${srcs})
