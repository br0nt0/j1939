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

static uint32_t getCANIDFromJ1939Messge( j1939Message_t message )
{
    uint32_t id = ( uint32_t ) message->sourceAddress;
    id |= ( uint32_t ) message->priority << 26u;
    id |= ( uint32_t ) ( message->parameterGroupNumber << 8u );
    if ( message->parameterGroupNumber < 0xf000u )
    {
        id &= ~( ( uint32_t ) 0xffU << 8U );
        id |= ( uint32_t ) message->destinationAddress << 8u;
    }
    return ( id );
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
            CANMessage_t canMessage = createCANMessage( getCANIDFromJ1939Messge( message ),
                                                        true,
                                                        message->data,
                                                        message->dataLength );
            status = sendCANMessage( driver, canMessage );
            destroyCANMessage( canMessage );
        }
    }
    return ( status );
}

j1939Message_t receiveJ1939MessageFromDriver( canDriver_t driver )
{
    j1939Message_t j1939Message = NULL;

    if ( driver != NULL )
    {
        CANMessage_t canMessage = receiveCANMessage( driver );
        if ( canMessage != NULL )
        {
            uint32_t id = getCANMessageID( canMessage );
            if ( ( ( id & EDP_BIT ) == 0u )
                && ( isCANMessageExtended( canMessage ) == true ) )
            {
                j1939Message = createJ1939Message(  extractPGN( id ),
                                                    ( uint8_t ) ( ( id >> 26u ) & 0x7u ),
                                                    extractDestinationAddress( id ),
                                                    ( uint8_t ) ( id & 0xffu ),
                                                    getCANMessageData( canMessage ),
                                                    getCANMessageDLC( canMessage ) );
            }
        }
    }

    return ( j1939Message );
}

