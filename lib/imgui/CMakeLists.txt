
cmake_minimum_required (VERSION 3.27)
project (imgui)

# ImGui source files
set(IMGUI_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(IMGUI_SOURCES
        ${IMGUI_SOURCE_DIR}/imgui.cpp
        ${IMGUI_SOURCE_DIR}/imgui_demo.cpp
        ${IMGUI_SOURCE_DIR}/imgui_draw.cpp
        ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
        ${IMGUI_SOURCE_DIR}/imgui_tables.cpp

)

# Add ImGui library
add_library(imgui ${IMGUI_SOURCES})

# Include directories
target_include_directories(imgui PUBLIC ${IMGUI_SOURCE_DIR})