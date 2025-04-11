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
#include "CANMessage.h"

#define CAN_GLOBAL_ADDRESS ( 255u )

enum
{
    CAN_DRIVER_IS_NULL = 0u,
    CAN_DRIVER_NOT_OPERATIONAL,
    CAN_TX_BUFFER_FULL,
    CAN_TX_SUCCEEDED,
    CAN_TX_ERROR_DETECTED,
    CAN_TX_MESSAGE_ABORTED,
    CAN_TX_MESSAGE_ARBITRATION_LOST,
    CAN_TX_MESSAGE_NOT_SENT
};

typedef struct canDriverStruct* canDriver_t;
typedef struct canDriverInterfaceStruct* canDriverInterface_t;
typedef struct canDriverStruct
{
    canDriverInterface_t vTable;
    const char* type;
}canDriverStruct_t;

typedef struct canDriverInterfaceStruct
{
    void ( *destroy )( canDriver_t );
    bool_t( *isOperational )( canDriver_t );
    uint8_t( *sendMessage )( canDriver_t, const CANMessage_t );
    CANMessage_t ( *receiveMessage )( canDriver_t );
    bool_t( *isTxBussOffState )( canDriver_t );
}canDriverInterfaceStruct_t;

/******************************************************************************/
void destroyCANDriver( canDriver_t self );
uint8_t sendCANMessage( canDriver_t self, const CANMessage_t message );
CANMessage_t receiveCANMessage( canDriver_t self );
bool_t isCANDriverOperational( canDriver_t self );
const char* getCANDriverType( canDriver_t driver );
bool_t isCANTxBusOffState( canDriver_t self );

#endif /* CANDRIVER_H_ */
