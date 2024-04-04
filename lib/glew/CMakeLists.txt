
cmake_minimum_required (VERSION 3.27)
project (glew)

# ImGui source files
set(GLEW_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(GLEW_SOURCES
        ${GLEW_SOURCE_DIR}/src/glew.c
        ${GLEW_SOURCE_DIR}/src/glewinfo.c
        ${GLEW_SOURCE_DIR}/src/visualinfo.c
)

# Add ImGui library
add_library(glew ${GLEW_SOURCES})

# Include directories
target_include_directories(glew PUBLIC ${GLEW_SOURCE_DIR}/include)