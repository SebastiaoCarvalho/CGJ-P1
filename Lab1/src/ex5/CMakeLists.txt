# Add the library for ex5
add_library(ex5 ex5.cpp)

# Specify include directories for ex5
target_include_directories(ex5 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ex5 PRIVATE utils)