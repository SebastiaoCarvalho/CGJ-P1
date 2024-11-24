# Add the library for utils
add_library(utils utils.cpp)

# Specify include directories for utils
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})