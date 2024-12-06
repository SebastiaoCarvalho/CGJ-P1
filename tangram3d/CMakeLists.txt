# Root CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# Set project name and version
project(mesh-loader VERSION 1.0 LANGUAGES CXX)

# Set default build type to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Copy asset folder to build directory
file(COPY ${CMAKE_SOURCE_DIR}/../assets DESTINATION ${CMAKE_BINARY_DIR})

# Add subdirectories
add_subdirectory(mgl)
add_subdirectory(src)
