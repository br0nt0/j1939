/*******************************************************************************
 * @file	j1939Message.h
 * @brief	
 * @author	@br0nt0
 * @date	2025
 ******************************************************************************/
#ifndef J1939MESSAGE_H_
#define J1939MESSAGE_H_
/******************************************************************************/
#include "commonTypes.h"
#include "drivers/CANDriver.h"

/******************************************************************************/
typedef struct j1939MessageStruct* j1939Message_t;

/******************************************************************************/
j1939Message_t createJ1939Message(  uint32_t parameterGroupNumber,
                                    uint8_t priority,
                                    uint8_t destinationAddress,
                                    uint8_t sourceAddress,
                                    uint8_t* data,
                                    uint8_t dataLength );
void destroyJ1939Message( j1939Message_t self );

uint32_t getJ1939MessagePGN( j1939Message_t self );
uint8_t getJ1939MessagePriority( j1939Message_t self );
uint8_t getJ1939MessageDA( j1939Message_t self );
uint8_t getJ1939MessageSA( j1939Message_t self );
uint8_t* getJ1939MessageData( j1939Message_t self );
uint8_t getJ1939MessageDataLength( j1939Message_t self );

uint8_t sendJ1939MessageToDriver( j1939Message_t message, canDriver_t driver );
j1939Message_t receiveJ1939MessageFromDriver( canDriver_t driver );


#endif /* J1939MESSAGE_H_ */
