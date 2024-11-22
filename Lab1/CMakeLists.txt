cmake_minimum_required(VERSION 3.10)

project(CGJ)

include_directories(${CMAKE_SOURCE_DIR}/libraries)
add_executable(cgj src/main.cpp)
