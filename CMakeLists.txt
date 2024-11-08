cmake_minimum_required(VERSION 3.13)
project(j1939 VERSION 0.1.0 LANGUAGES C CXX)

set(PRJ_ROOT_DIR        ${CMAKE_CURRENT_SOURCE_DIR})
set(PRJ_DIR             ${PRJ_ROOT_DIR}/src)
set(PRJ_INCLUDE_DIR     ${PRJ_DIR})

set(EXECUTABLE          ${CMAKE_PROJECT_NAME})

add_subdirectory(src)
add_subdirectory(tests)

