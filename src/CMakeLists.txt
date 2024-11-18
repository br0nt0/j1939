cmake_minimum_required(VERSION  3.13.0)

add_library(${EXECUTABLE})
target_include_directories(${EXECUTABLE} PRIVATE
                                ${PRJ_INCLUDE_DIR})

add_subdirectory(drivers)
add_subdirectory(j1939)