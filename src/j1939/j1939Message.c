/*******************************************************************************
 * @file	j1939Message.c
 * @brief	
 * @author	@br0nt0
 * @date	2025
 ******************************************************************************/
#include "j1939Message.h"

/******************************************************************************/
#define PGN_MASK    ( 0x1ffffu )
#define PF_MASK     ( 0xff00u )
#define EDP_BIT     ( 0x2000000u )
typedef struct j1939MessageStruct
{
    uint32_t parameterGroupNumber;
    uint8_t priority;
    uint8_t destinationAddress;
    uint8_t sourceAddress;
    uint8_t dataLength;
    uint8_t* data;
}j1939MessageStruct_t;

/******************************************************************************/
static void convertToCANMessage( canMessage_t canMessage, const j1939Message_t j1939Message )
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
    canMessage->dlc = ( j1939Message->dataLength > 8U ) ? 8U : ( uint8_t ) j1939Message->dataLength;
    canMessage->data = j1939Message->data;
}

static uint8_t extractDestinationAddress( uint32_t id )
{
    uint8_t destinationAddress = CAN_GLOBAL_ADDRESS;
    if ( ( ( id >> 16u ) & 0xffu ) < 0xf0u )
    {
        destinationAddress = ( uint8_t ) ( ( id >> 8u ) & 0xffu );
    }
    return ( destinationAddress );
}

static uint32_t extractPGN( uint32_t id )
{
    uint32_t pgn = ( uint32_t ) ( ( id >> 8u ) & PGN_MASK );
    if ( ( pgn & PF_MASK ) < 0xf000u )
    {
        pgn &= 0x1ff00u;
    }
    return ( pgn );
}

/******************************************************************************/
j1939Message_t createJ1939Message(  uint32_t parameterGroupNumber,
                                    uint8_t priority,
                                    uint8_t destinationAddress,
                                    uint8_t sourceAddress,
                                    uint8_t* data,
                                    uint8_t dataLength )
{
    j1939Message_t self = NULL;
    if ( ( parameterGroupNumber != 0u ) && ( data != NULL ) && ( dataLength > 0u ) )
    {
        self = ( j1939Message_t ) malloc( sizeof( j1939MessageStruct_t ) );
        if ( self != NULL )
        {
            self->parameterGroupNumber = parameterGroupNumber;
            self->priority = priority;
            self->destinationAddress = destinationAddress;
            self->sourceAddress = sourceAddress;
            self->dataLength = dataLength;
            self->data = data;
        }
    }
    return ( self );
}

void destroyJ1939Message( j1939Message_t self )
{
    if( self != NULL )
    {
        free( self );
    }
}

uint32_t getJ1939MessagePGN( j1939Message_t self )
{
    return ( self->parameterGroupNumber );
}

uint8_t getJ1939MessagePriority( j1939Message_t self )
{
    return ( self->priority );
}

uint8_t getJ1939MessageDA( j1939Message_t self )
{
    return ( self->destinationAddress );
}

uint8_t getJ1939MessageSA( j1939Message_t self )
{
    return ( self->sourceAddress );
}

uint8_t* getJ1939MessageData( j1939Message_t self )
{
    return ( self->data );
}

uint8_t getJ1939MessageDataLength( j1939Message_t self )
{
    return ( self->dataLength );
}

uint8_t sendJ1939MessageToDriver( j1939Message_t message, canDriver_t driver )
{
    uint8_t status = CAN_TX_MESSAGE_NOT_SENT;
    if ( message != NULL )
    {
        status = CAN_DRIVER_NOT_OPERATIONAL;
        if ( isCANDriverOperational( driver ) )
        {
            canMessage_t canMessage = ( canMessage_t ) malloc( sizeof( canMessageStruct_t ) );
            convertToCANMessage( canMessage, message );
            status = sendCANMessage( driver, canMessage );
            free( canMessage );
        }
    }
    return ( status );
}

j1939Message_t receiveJ1939MessageFromDriver( canDriver_t driver )
{
    j1939Message_t j1939Message = NULL;

    if ( driver != NULL )
    {
        canMessage_t canMessage = receiveCANMessage( driver );
        if ( canMessage != NULL )
        {
            if ( ( ( canMessage->id & EDP_BIT ) == 0u )
                && ( canMessage->isExtended == true ) )
            {
                j1939Message = createJ1939Message(  extractPGN( canMessage->id ),
                                                    ( uint8_t ) ( ( canMessage->id >> 26u ) & 0x7u ),
                                                    extractDestinationAddress(canMessage->id),
                                                    ( uint8_t ) ( canMessage->id & 0xffu ),
                                                    canMessage->data,
                                                    canMessage->dlc );
            }
        }
    }

    return ( j1939Message );
}

