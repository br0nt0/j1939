/*******************************************************************************
 * @file	messageConverter.c
 * @brief
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "messageConverter.h"
/******************************************************************************/
#define PGN_MASK    ( 0x1ffffu )
#define PF_MASK     ( 0xff00u )
#define EDP_BIT     ( 1u << 25u )
#define DP_BIT      ( 1u << 24u )
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

void decodeMessage( j1939Message_t j1939Message, const canMessage_t canMessage )
{
    if ( j1939Message && canMessage )
    {
        bool_t isEDPBitSet = ( canMessage->id & EDP_BIT ) > 0u;
        if ( ( canMessage->isExtended ) && !isEDPBitSet )
        {
            if ( ( ( canMessage->id >> 16u ) & 0xffu ) < 0xf0u )
            {
                j1939Message->destinationAddress = ( uint8_t ) ( ( canMessage->id >> 8u ) & 0xffu );
            }
            else
            {
                j1939Message->destinationAddress = CAN_GLOBAL_ADDRESS;
            }

            uint32_t pgn = ( uint32_t ) ( ( canMessage->id >> 8u ) & PGN_MASK );
            if ( ( pgn & PF_MASK ) < 0xf000u )
            {
                pgn &= 0x1ff00u;
            }
            j1939Message->parameterGroupNumber = pgn;

            j1939Message->sourceAddress = ( uint8_t ) ( canMessage->id & 0xffu );
            j1939Message->priority = ( uint8_t ) ( ( canMessage->id >> 26u ) & 0xffu );
            j1939Message->dataSize = ( uint16_t ) canMessage->dlc;
            j1939Message->data = canMessage->data;
        }
        else
        {
            j1939Message->parameterGroupNumber = 0u;
            j1939Message->priority = 0u;
            j1939Message->sourceAddress = 0u;
            j1939Message->destinationAddress = 0u;
            j1939Message->dataSize = 0u;
            j1939Message->data = NULL;
        }
    }
}

