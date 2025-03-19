cmake_minimum_required(VERSION  3.13.0)
target_sources(${TEST_EXECUTABLE} PRIVATE 
                                ${PRJ_UNIT_TEST_DIR}/j1939/j1939StackTests.cpp
                                ${PRJ_UNIT_TEST_DIR}/j1939/messageConverterTests.cpp
                                ${PRJ_UNIT_TEST_DIR}/j1939/messageSendTests.cpp
                                ${PRJ_UNIT_TEST_DIR}/j1939/j1939StackInstanceTests.cpp
                                ${PRJ_UNIT_TEST_DIR}/j1939/messageReceiveTests.cpp
                                ${PRJ_UNIT_TEST_DIR}/j1939/addressClaimedTests.cpp
                                ${PRJ_UNIT_TEST_DIR}/j1939/iAddressClaimedTests.cpp
                                ${PRJ_UNIT_TEST_DIR}/j1939/addressClaimedImplTests.cpp)