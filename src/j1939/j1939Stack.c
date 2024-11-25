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
