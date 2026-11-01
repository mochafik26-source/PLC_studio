
cmake_minimum_required(VERSION 3.16)

project(PLC_GUI LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(SQLite3 REQUIRED)

set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/external/imgui")

add_executable(PLC_GUI
    src/main.cpp
    src/addingVar.cpp

    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp

    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

target_compile_options(PLC_GUI PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-include>
    $<$<COMPILE_LANGUAGE:CXX>:cstdarg>
)

target_include_directories(PLC_GUI PRIVATE
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

target_link_libraries(PLC_GUI PRIVATE
    glfw
    OpenGL::GL
    SQLite::SQLite3
)
