# Add the library for ex3
add_library(ex3 ex3.cpp)

# Specify include directories for ex3
target_include_directories(ex3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ex3 PRIVATE utils)