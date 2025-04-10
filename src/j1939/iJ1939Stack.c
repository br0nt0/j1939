/*******************************************************************************
 * @file	iJ1939Stack.c
 * @brief   interface of J1939 stack
 * @author  @br0nt0
 * @date	2024
 ******************************************************************************/
#include "iJ1939Stack.h"
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

void updateJ1939CoreScheduler( j1939_t self )
{
    if ( NULL != self )
    {
        self->iFace->updateCoreScheduler( self );
    }
}

bool_t wasJ1939AddressClaimed( j1939_t self )
{
    bool_t wasAddressClaimed = false;
    if ( NULL != self )
    {
        wasAddressClaimed = self->iFace->wasAddressClaimed( self );
    }
    return ( wasAddressClaimed );
}

