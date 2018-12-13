cmake_minimum_required(VERSION 3.1)
project(dynamic_nested_cages)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# libigl
option(LIBIGL_WITH_OPENGL            "Use OpenGL"         ON)
option(LIBIGL_WITH_OPENGL_GLFW       "Use GLFW"           ON)
option(LIBIGL_WITH_VIEWER            "Use OpenGL viewer"  ON)

find_package(LIBIGL REQUIRED QUIET)

file(GLOB SRCFILES *.cpp)
add_executable(gen_cage ${SRCFILES})
target_link_libraries(gen_cage igl::core igl::opengl_glfw)
