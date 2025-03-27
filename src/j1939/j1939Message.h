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

/******************************************************************************/
typedef struct j1939MessageStruct* j1939Message_t;

/******************************************************************************/
j1939Message_t createJ1939Message(  uint16_t parameterGroupNumber,
                                    uint8_t priority,
                                    uint8_t destinationAddress,
                                    uint8_t sourceAddress,
                                    uint8_t* data,
                                    uint8_t dataLength );
void destroyJ1939Message( j1939Message_t self );
uint16_t getPGN( j1939Message_t self );
uint8_t getPriority( j1939Message_t self );
uint8_t getDA( j1939Message_t self );
uint8_t getSA( j1939Message_t self );
uint8_t* getData( j1939Message_t self );
uint8_t getDataLength( j1939Message_t self );

#endif /* J1939MESSAGE_H_ */
