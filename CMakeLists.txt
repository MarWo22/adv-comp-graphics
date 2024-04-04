cmake_minimum_required(VERSION 3.27)
project(main)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib/glfw)
add_subdirectory(lib/glm)
add_subdirectory(lib/imgui)

# Add GLEW include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/glew/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/glew/lib)  # Assuming the compiled libraries are in the lib folder


set(STB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/lib/stb)
include_directories(${STB_INCLUDE_DIR})

add_executable(main
        src/cpp/main.cpp
        src/cpp/gl_misc.cpp
        src/cpp/shading.cpp
        src/cpp/imgui/imgui_impl_glfw.cpp
        src/cpp/imgui/imgui_impl_opengl3.cpp
 )

find_package(OpenGL REQUIRED)

target_link_libraries(main PRIVATE
        glfw
        glm
        imgui
        glew32
        glu32
        ${OPENGL_gl_LIBRARY}
)

# Set the build directory to 'build' within the project directory
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Copy shader files to build directory
file(GLOB SHADER_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/*
)

foreach(SHADER_FILE ${SHADER_FILES})
    get_filename_component(FILE_NAME ${SHADER_FILE} NAME)
    configure_file(${SHADER_FILE} ${CMAKE_BINARY_DIR}/shaders/${FILE_NAME} COPYONLY)
endforeach()