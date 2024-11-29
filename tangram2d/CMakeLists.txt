# Root CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# Set project name and version
project(hello-2d-world VERSION 1.0 LANGUAGES CXX)

# Set default build type to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Add subdirectories
add_subdirectory(mgl)
add_subdirectory(src)
