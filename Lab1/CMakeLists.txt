cmake_minimum_required(VERSION 3.10)

project(LAB1)

include_directories(${CMAKE_SOURCE_DIR}/libraries)

add_subdirectory(src/utils)
add_subdirectory(src/ex1)
add_subdirectory(src/ex2)
add_subdirectory(src/ex3)
add_subdirectory(src/ex4)
add_subdirectory(src/ex5)
add_subdirectory(src/ex6)

add_executable(lab1 src/main.cpp)

target_link_libraries(lab1 ex1 ex2 ex3 ex4 ex5 ex6)
