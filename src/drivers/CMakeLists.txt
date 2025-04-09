cmake_minimum_required(VERSION  3.13.0)
target_sources(${EXECUTABLE} PRIVATE 
                                ${PRJ_DIR}/drivers/CANDriver.c
                                ${PRJ_DIR}/drivers/CANMessage.c)