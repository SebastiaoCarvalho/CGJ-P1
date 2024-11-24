# Add the library for ex6
add_library(ex6 ex6.cpp)

# Specify include directories for ex6
target_include_directories(ex6 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ex6 PRIVATE utils)