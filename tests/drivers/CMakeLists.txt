cmake_minimum_required(VERSION  3.13.0)
target_sources(${TEST_EXECUTABLE} PRIVATE 
                                ${PRJ_UNIT_TEST_DIR}/drivers/CANDriverTests.cpp
                                ${PRJ_UNIT_TEST_DIR}/drivers/CANDriverSpy.cpp
                                ${PRJ_UNIT_TEST_DIR}/drivers/CANDriverSpyTests.cpp
                                ${PRJ_UNIT_TEST_DIR}/drivers/CANMessageTests.cpp)