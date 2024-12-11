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

uint8_t sendCANMessage( canDriver_t self, const canMessage_t message )
{
    uint8_t result = CAN_DRIVER_IS_NULL;
    if ( NULL != self )
    {
        result = self->vTable->sendMessage( self, message );
    }
    
    return ( result );
}

canMessage_t receiveCANMessage( canDriver_t self )
{
    canMessage_t message = NULL;
    if ( NULL != self )
    {
        message = self->vTable->receiveMessage( self );
    }
    
    return ( message );
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

bool_t isCANTxBusOffState( canDriver_t self )
{
    bool_t isBussOff = false;
    if ( NULL != self )
    {
        isBussOff = self->vTable->isTxBussOffState( self );
    }
    return ( isBussOff );
}

const char* getCANDriverType( canDriver_t driver )
{
    return ( driver->type );
}