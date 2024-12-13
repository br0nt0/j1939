/*******************************************************************************
 * @file	j1939Stack.c
 * @brief
 * @author  @br0nt0
 * @date	2024
 ******************************************************************************/
#include "j1939Stack.h"
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
void destroyJ1939Stack( j1939_t self )
{
    if ( NULL != self )
    {
        self->iFace->destroy( self );
    }
}

uint8_t sendJ1939Message( j1939_t self, const j1939Message_t message )
{
    uint8_t status = 1u;

    if ( NULL != self )
    {
        status = self->iFace->sendJ1939Message( self, message );
    }
    return ( status );
}

const char* getJ1939StackType( j1939_t self )
{
    return ( self->type );
}

j1939Message_t receiveJ1939Message( j1939_t self )
{
    j1939Message_t message = NULL;
    if ( NULL != self )
    {
        message = self->iFace->receiveJ1939Message( self );
    }
    return ( message );
}

uint8_t getJ1939SourceAddress( j1939_t self )
{
    uint8_t address = 0xffu;
    if ( NULL != self )
    {
        address = self->iFace->getSourceAddress( self );
    }
    return ( address );
}

void setJ1939SourceAddress( j1939_t self, uint8_t address )
{
    if ( NULL != self )
    {
        self->iFace->setSourceAddress( self, address );
    }
}

uint8_t* getJ1939CAName( j1939_t self )
{
    uint8_t* caName = NULL;
    if ( NULL != self )
    {
        caName = self->iFace->getCAName( self );
    }    
    return ( caName );
}

void setJ1939CAName( j1939_t self, const uint8_t* caName )
{
    if ( NULL != self )
    {
        self->iFace->setCAName( self, caName );
    }
}

canDriver_t getJ1939ConfiguredCANDriver( j1939_t self )
{
    canDriver_t driver = NULL;
    if ( NULL != self )
    {
        driver = self->iFace->getConfiguredCANDriver( self );
    }
    return ( driver );
}

uint8_t getJ1939ConfiguredTickMs( j1939_t self )
{
    uint8_t tickMs = 0u;
    if ( NULL != self )
    {
        tickMs = self->iFace->getTickMs( self );
    }
    return ( tickMs );
}


