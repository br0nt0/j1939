/*******************************************************************************
 * @file	j1939Stack.h
 * @brief
 * @author  @br0nt0
 * @date	2024
 ******************************************************************************/
#ifndef J1939STACK_H_
#define J1939STACK_H_
/******************************************************************************/
#include "commonTypes.h"
#include "drivers/CANDriver.h"

typedef struct j1939Struct* j1939_t;
typedef struct j1939MessageStruct* j1939Message_t;

/******************************************************************************/
void destroyJ1939Stack( j1939_t self );
uint8_t sendJ1939Message( j1939_t self, const j1939Message_t message );
j1939Message_t receiveJ1939Message( j1939_t self );
const char* getJ1939StackType( j1939_t self );
uint8_t getJ1939SourceAddress( j1939_t self );
void setJ1939SourceAddress( j1939_t self, uint8_t address );
uint8_t* getJ1939CAName( j1939_t self );
void setJ1939CAName( j1939_t self, const uint8_t* caName );
canDriver_t getJ1939ConfiguredCANDriver( j1939_t self );
uint8_t getJ1939ConfiguredTickMs( j1939_t self );

#include "j1939StackInterface.h"

#endif /* J1939STACK_H_ */
