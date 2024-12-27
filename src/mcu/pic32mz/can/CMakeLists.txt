cmake_minimum_required(VERSION  3.13.0)
target_sources(${EXECUTABLE} PRIVATE 
                                ${PIC32MZ_DIR}/can/foo.c)