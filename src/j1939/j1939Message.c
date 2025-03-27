/*******************************************************************************
 * @file	j1939Message.c
 * @brief	
 * @author	@br0nt0
 * @date	2025
 ******************************************************************************/
#include "j1939Message.h"

/******************************************************************************/
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

/******************************************************************************/
j1939Message_t createJ1939Message(  uint16_t parameterGroupNumber,
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

uint16_t getPGN( j1939Message_t self )
{
    return ( self->parameterGroupNumber );
}

uint8_t getPriority( j1939Message_t self )
{
    return ( self->priority );
}

uint8_t getDA( j1939Message_t self )
{
    return ( self->destinationAddress );
}

uint8_t getSA( j1939Message_t self )
{
    return ( self->sourceAddress );
}

uint8_t* getData( j1939Message_t self )
{
    return ( self->data );
}

uint8_t getDataLength( j1939Message_t self )
{
    return ( self->dataLength );
}