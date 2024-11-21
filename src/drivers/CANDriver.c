/*******************************************************************************
 * @file	CANDriver.c
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "CANDriver.h"
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
void destroyCANDriver( canDriver_t self )
{
    if ( NULL != self )
    {
        self->vTable->destroy( self );
    }
}

uint8_t sendCANMessage( canDriver_t self, canMessage_t message )
{
    ( void ) self;
    ( void ) message;
    return 0u;
}

bool_t isCANDriverOperational( canDriver_t self )
{
    bool_t isOperational = false;
    if ( NULL != self )
    {
        isOperational = self->vTable->isOperational( self );
    }
    return ( isOperational );
}

const char* getCANDriverType( canDriver_t driver )
{
    return ( driver->type );
}