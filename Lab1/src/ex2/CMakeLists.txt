# Add the library for ex2
add_library(ex2 ex2.cpp)

# Specify include directories for ex2
target_include_directories(ex2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ex2 PRIVATE utils)