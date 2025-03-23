cmake_minimum_required(VERSION  3.13.0)
target_sources(${EXECUTABLE} PRIVATE 
                                ${PRJ_DIR}/j1939/j1939Stack.c
                                ${PRJ_DIR}/j1939/messageConverter.c
                                ${PRJ_DIR}/j1939/messageSend.c
                                ${PRJ_DIR}/j1939/messageReceive.c
                                ${PRJ_DIR}/j1939/j1939StackInstance.c
                                ${PRJ_DIR}/j1939/iAddressClaimed.c
                                ${PRJ_DIR}/j1939/addressClaimedImpl.c)