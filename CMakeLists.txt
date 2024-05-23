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
        src/cpp/shader.cpp
        src/cpp/imgui/imgui_impl_glfw.cpp
        src/cpp/imgui/imgui_impl_opengl3.cpp
        src/cpp/textures/texture.cpp
        src/cpp/textures/texture.h
        src/cpp/textures/mipmap.cpp
        src/cpp/textures/mipmap.h
        src/cpp/textures/image.cpp
        src/cpp/textures/image.h
        src/cpp/Camera.cpp
        src/cpp/Camera.h
        src/cpp/sky/sky_physics.cpp
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

# Get all files in the 'assets' directory and its subdirectories
file(GLOB_RECURSE ASSET_FILES_AND_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/assets/*)

foreach(ASSET_FILE_OR_DIR ${ASSET_FILES_AND_DIRS})
    # Get the relative path of the file or directory
    file(RELATIVE_PATH REL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/assets ${ASSET_FILE_OR_DIR})

    if(IS_DIRECTORY ${ASSET_FILE_OR_DIR})
        # If it's a directory, create it in the destination directory
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/assets/${REL_PATH})
    else()
        # If it's a file, copy it to the destination directory
        configure_file(${ASSET_FILE_OR_DIR} ${CMAKE_BINARY_DIR}/assets/${REL_PATH} COPYONLY)
    endif()
endforeach()