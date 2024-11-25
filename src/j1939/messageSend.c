/*******************************************************************************
 * @file	messageSend.c
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "messageSend.h"
#include "messageConverter.h"

/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
uint8_t sendJ1939MessageToCANDriver( j1939Message_t message, canDriver_t driver )
{
    static canMessageStruct_t stackMemory;
    uint8_t status = CAN_DRIVER_NOT_OPERATIONAL;
    if ( isCANDriverOperational( driver ) )
    {
        canMessage_t canMessage = &stackMemory;
        encodeCANMessage( canMessage, message );
        status = sendCANMessage( driver, canMessage );
    }

    return( status );
}