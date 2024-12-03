# src/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# Create the executable
project(hello-3d-world LANGUAGES CXX)

# Source files
set(SRC_SOURCES hello-3d-world.cpp)

# GLSL shader files
set(SHADER_FILES color-fs.glsl color-vs.glsl)

# Add the executable
add_executable(${PROJECT_NAME} ${SRC_SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    /usr/include
    ${CMAKE_SOURCE_DIR}/mgl
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    mgl
    OpenGL
    glfw
    GLEW
    assimp
)

# Runtime library path
set_target_properties(${PROJECT_NAME} PROPERTIES
    BUILD_RPATH ${CMAKE_SOURCE_DIR}/mgl
)

# Copy GLSL files to the build directory
foreach(SHADER_FILE ${SHADER_FILES})
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${SHADER_FILE}
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER_FILE}
                ${CMAKE_CURRENT_BINARY_DIR}/${SHADER_FILE}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${SHADER_FILE}
    )
endforeach()

# Ensure shaders are copied before building the executable
add_custom_target(copy_shaders ALL DEPENDS ${SHADER_FILES})
add_dependencies(${PROJECT_NAME} copy_shaders)

# Custom target for "run"
add_custom_target(run
    COMMAND LD_LIBRARY_PATH=${CMAKE_SOURCE_DIR}/mgl ./${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
