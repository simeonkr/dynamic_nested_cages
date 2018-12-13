cmake_minimum_required(VERSION 3.1)
project(dynamic_nested_cages)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# libigl
option(LIBIGL_WITH_OPENGL            "Use OpenGL"         ON)
option(LIBIGL_WITH_OPENGL_GLFW       "Use GLFW"           ON)
option(LIBIGL_WITH_VIEWER            "Use OpenGL viewer"  ON)

find_package(LIBIGL REQUIRED QUIET)

add_library(common extrap_cage.cpp mvc.cpp)
add_executable(gen_cage main.cpp)
add_executable(run_tests run_tests.cpp)
target_link_libraries(common igl::core igl::opengl_glfw)
target_link_libraries(gen_cage common)
target_link_libraries(run_tests common)
