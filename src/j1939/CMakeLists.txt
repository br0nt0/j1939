cmake_minimum_required(VERSION  3.13.0)
target_sources(${EXECUTABLE} PRIVATE 
                                ${PRJ_DIR}/j1939/iJ1939Stack.c
                                ${PRJ_DIR}/j1939/j1939StackImpl.c
                                ${PRJ_DIR}/j1939/iAddressClaimed.c
                                ${PRJ_DIR}/j1939/addressClaimedImpl.c
                                ${PRJ_DIR}/j1939/j1939Message.c)