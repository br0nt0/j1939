cmake_minimum_required(VERSION  3.13.0)
target_sources(${TEST_EXECUTABLE} PRIVATE 
                                ${PRJ_UNIT_TEST_DIR}/j1939/iJ1939StackTests.cpp
                                ${PRJ_UNIT_TEST_DIR}/j1939/j1939StackImplTests.cpp
                                ${PRJ_UNIT_TEST_DIR}/j1939/iAddressClaimedTests.cpp
                                ${PRJ_UNIT_TEST_DIR}/j1939/addressClaimedImplTests.cpp
                                ${PRJ_UNIT_TEST_DIR}/j1939/j1939MessageTests.cpp
                                ${PRJ_UNIT_TEST_DIR}/j1939/transportProtocolPackageTests.cpp)