# Add the library for ex4
add_library(ex4 ex4.cpp)

# Specify include directories for ex4
target_include_directories(ex4 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ex4 PRIVATE utils)