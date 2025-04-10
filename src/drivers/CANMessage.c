/*******************************************************************************
 * @file	CANMessage.c
 * @brief	
 * @author	@br0nt0
 * @date	2025
 ******************************************************************************/
#include "CANMessage.h"
/******************************************************************************/
typedef struct CANMessageStruct
{
    uint32_t id;
    bool_t isExtended;
    uint8_t dlc;
    uint8_t* data;
}CANMessageStruct_t;

/******************************************************************************/
static bool_t isInputDataValid( const uint8_t* data, uint8_t dlc )
{
    return ( ( data != NULL ) && ( dlc > 0u ) );
}

/******************************************************************************/
void destroyCANMessage( CANMessage_t self )
{
    if ( self != NULL )
    {
        free( self );
    }
}

bool_t isCANMessageExtended( CANMessage_t self )
{
    bool_t isExtended = false;
    if ( self != NULL )
    {
        isExtended = self->isExtended;
    }
    return ( isExtended );
}

uint8_t* getCANMessageData( CANMessage_t self )
{
    uint8_t* data = NULL;
    if ( self != NULL )
    {
        data = self->data;
    }
    return ( data );
}

uint8_t getCANMessageDLC( CANMessage_t self )
{
    uint8_t dlc = 0u;
    if ( self != NULL )
    {
        dlc = self->dlc;
    }
    return ( dlc );
}

uint32_t getCANMessageID( CANMessage_t self )
{
    uint32_t id = 0u;
    if ( self != NULL )
    {
        id = self->id;
    }
    return ( id );
}

CANMessage_t createExtendedCANMessage( uint32_t id, uint8_t* data, uint8_t dlc )
{
    CANMessage_t self = NULL;
    if ( isInputDataValid( data, dlc ) )
    {
        self = ( CANMessage_t ) malloc( sizeof( CANMessageStruct_t ) );
        self->isExtended = true;
        self->id = id;
        self->data = data;
        self->dlc = dlc;
    }
    return ( self );
}

CANMessage_t createStandardCANMessage( uint32_t id, uint8_t* data, uint8_t dlc )
{
    CANMessage_t self = NULL;
    if ( isInputDataValid( data, dlc )
        && ( id < 0x7ffu ) )
    {
        self = ( CANMessage_t ) malloc( sizeof( CANMessageStruct_t ) );
        self->isExtended = false;
        self->id = id;
        self->data = data;
        self->dlc = dlc;
    }
    return ( self );
}


