cmake_minimum_required(VERSION  3.13.0)
target_sources(${TEST_EXECUTABLE} PRIVATE 
                                ${PIC32MZ_TEST_DIR}/can/canRegisterIOMock.cpp
                                ${PIC32MZ_TEST_DIR}/can/canRegisterIOMockTests.cpp
                                ${PIC32MZ_TEST_DIR}/can/canTests.cpp)
