/*******************************************************************************
 * @file	CANDriver.h
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#ifndef CANDRIVER_H_
#define CANDRIVER_H_
/******************************************************************************/
#include "commonTypes.h"

#define CAN_GLOBAL_ADDRESS ( 255u )

enum
{
    CAN_DRIVER_IS_NULL = 0u,
    CAN_DRIVER_NOT_OPERATIONAL,
    CAN_TX_BUFFER_FULL,
    CAN_TX_SUCCEEDED
};


typedef struct canDriverStruct* canDriver_t;
typedef struct canMessageStruct* canMessage_t;

/******************************************************************************/
void destroyCANDriver( canDriver_t self );
uint8_t sendCANMessage( canDriver_t self, const canMessage_t message );
canMessage_t receiveCANMessage( canDriver_t self );
bool_t isCANDriverOperational( canDriver_t self );
const char* getCANDriverType( canDriver_t driver );
bool_t isCANTxBusOffState( canDriver_t self );



#include "CANDriverInterface.h"

#endif /* CANDRIVER_H_ */
