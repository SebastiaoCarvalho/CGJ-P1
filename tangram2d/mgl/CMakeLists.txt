# mgl/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# Create the mgl library
project(mgl LANGUAGES CXX)

# Add library source files
file(GLOB_RECURSE MGL_SOURCES *.cpp)
file(GLOB_RECURSE MGL_HEADERS *.hpp)

# Add the library
add_library(mgl SHARED ${MGL_SOURCES} ${MGL_HEADERS})

# Include directories
target_include_directories(mgl PUBLIC
    /usr/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link libraries
target_link_libraries(mgl PUBLIC
    OpenGL
    glfw
    GLEW
    assimp
)
