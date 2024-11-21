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

enum
{
    CAN_DRIVER_IS_NULL = 0u,
    CAN_DRIVER_NOT_OPERATIONAL
};

typedef struct canDriverStruct* canDriver_t;
typedef struct canMessageStruct* canMessage_t;

/******************************************************************************/
void destroyCANDriver( canDriver_t self );
uint8_t sendCANMessage( canDriver_t self, canMessage_t message );
bool_t isCANDriverOperational( canDriver_t self );
const char* getCANDriverType( canDriver_t driver );


#include "CANDriverInterface.h"

#endif /* CANDRIVER_H_ */
