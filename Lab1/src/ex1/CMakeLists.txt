# Add the library for ex1
add_library(ex1 ex1.cpp)

# Specify include directories for ex1
target_include_directories(ex1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ex1 PRIVATE utils)