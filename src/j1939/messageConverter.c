/*******************************************************************************
 * @file	messageConverter.c
 * @brief
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "messageConverter.h"
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
void encodeCANMessage( canMessage_t canMessage, const j1939Message_t j1939Message )
{
    if ( canMessage && j1939Message )
    {
        canMessage->id = ( uint32_t ) j1939Message->sourceAddress;
        canMessage->id |= ( uint32_t ) j1939Message->priority << 26u;
        canMessage->id |= ( uint32_t ) ( j1939Message->parameterGroupNumber << 8u );
        if ( j1939Message->parameterGroupNumber < 0xf000u )
        {
            canMessage->id &= ~( ( uint32_t ) 0xffU << 8U );
            canMessage->id |= ( uint32_t ) j1939Message->destinationAddress << 8u;
        }
        canMessage->isExtended = true;
        canMessage->dlc = ( j1939Message->dataSize > 8U ) ? 8U : ( uint8_t ) j1939Message->dataSize;
        canMessage->data = j1939Message->data;
    }

}
