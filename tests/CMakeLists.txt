cmake_minimum_required(VERSION 3.13.0)

find_package(PkgConfig REQUIRED)
pkg_search_module(CPPUTEST REQUIRED cpputest>=3.8)
message(STATUS "Found CppUTest version ${CPPUTEST_VERSION}")

set(CPPUTEST_LDFLAGS                "${CPPUTEST_LDFLAGS} -pthread")

set(COMMON_COMPILE_FLAGS            -O0 
                                    -g
                                    -Wall
                                    -Wextra
                                    -Werror
                                    -Wpedantic
                                    -Wshadow
                                    -Wdouble-promotion
                                    -Wformat=2 -Wformat-truncation
                                    -Wundef
                                    -Wno-sign-conversion
                                    $<$<COMPILE_LANGUAGE:CXX>:
										-Wconversion
										-Wno-volatile>)

set(COMMON_DEFINES                  TESTING
                                    UNICODE)

# application library
target_compile_definitions(${EXECUTABLE} PRIVATE
                                    ${COMMON_DEFINES})
target_include_directories(${EXECUTABLE} PRIVATE
                                    ${PRJ_INCLUDE_DIR})
target_compile_options(${EXECUTABLE} PRIVATE
                                    ${COMMON_COMPILE_FLAGS}
                                    --coverage)
target_link_libraries(${EXECUTABLE} "--coverage")


# test executable
set(TEST_EXECUTABLE                 ${EXECUTABLE}_uTests)
add_executable(${TEST_EXECUTABLE})

set(UT_DIR			                ${PRJ_ROOT_DIR}/tests)
target_sources(${TEST_EXECUTABLE} PRIVATE
                                    ${UT_DIR}/uTestMain.cpp)
add_subdirectory(drivers)
add_subdirectory(j1939)


target_compile_definitions(${TEST_EXECUTABLE} PRIVATE
                                    ${COMMON_DEFINES})
target_include_directories(${TEST_EXECUTABLE} PRIVATE
                                    ${CPPUTEST_INCLUDE_DIRS}
                                    ${PRJ_INCLUDE_DIR}
                                    ${PRJ_UNIT_TEST_DIR})
target_compile_options(${TEST_EXECUTABLE} PRIVATE
                                    ${COMMON_COMPILE_FLAGS})
target_link_directories(${TEST_EXECUTABLE} PRIVATE
                                    ${CPPUTEST_LIBRARIES})
target_link_libraries(${TEST_EXECUTABLE} PRIVATE
                                    ${EXECUTABLE}
                                    ${CPPUTEST_LDFLAGS})

# Run the test once the build is done
add_custom_command(TARGET ${TEST_EXECUTABLE} COMMAND ./${TEST_EXECUTABLE} -c POST_BUILD)

# add custom target to run gcovr for coverage
set(OPEN_APPLICATION )
if (UNIX)
    set(OPEN_APPLICATION firefox)
endif (UNIX)

add_custom_command(OUTPUT run_gcovr_parser POST_BUILD
                    COMMAND gcovr   --root ${CMAKE_SOURCE_DIR}
                                    --txt-metric branch
                                    --html-details
                                    --output ${CMAKE_BINARY_DIR}/coverage.html
                                    --cobertura ${CMAKE_BINARY_DIR}/coverage.xml
                                    --cobertura-pretty
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                    COMMENT "Running gcovr to produce XML and HTML coverage reports")               
add_custom_target(coverage DEPENDS ${TEST_EXECUTABLE} run_gcovr_parser)
add_custom_command(TARGET coverage COMMAND ${OPEN_APPLICATION} ${CMAKE_BINARY_DIR}/coverage.html POST_BUILD)


# add custom target to run lizard for code complexity
add_custom_command(OUTPUT run_lizard_parser POST_BUILD
                    COMMAND lizard ../../src
                    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                    COMMENT "Running Lizard to produce code complexity report")
add_custom_target(complexity DEPENDS ${TEST_EXECUTABLE} run_lizard_parser)  