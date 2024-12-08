/*******************************************************************************
 * @file	messageReceive.c
 * @brief
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "messageReceive.h"
#include "messageConverter.h"

/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
j1939Message_t receiveJ1939MessageFromCANDriver( canDriver_t driver )
{
    static j1939MessageStruct_t j1939MessageMemory;
    j1939Message_t j1939Message = NULL;

    if ( driver != NULL )
    {
        canMessage_t canMessage = receiveCANMessage( driver );
        if ( canMessage != NULL )
        {
            j1939Message = &j1939MessageMemory;
            decodeMessage( j1939Message, canMessage );
        }
    }

    return ( j1939Message );
}

