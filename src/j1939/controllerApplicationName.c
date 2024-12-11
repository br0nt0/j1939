/*******************************************************************************
 * @file	controllerApplicationName.c
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "controllerApplicationName.h"

/******************************************************************************/
static uint8_t caNameStorage[ 8 ];
static const uint8_t* caName = NULL;

/******************************************************************************/

/******************************************************************************/
const uint8_t* getControllerApplicationName( void )
{
    return ( caName );
}

void setControllerapplicationName( const uint8_t* name )
{
    caName = NULL;
    if ( NULL != name )
    {
        for ( uint8_t i = 0; i < 8u; i++ )
        {
            caNameStorage[ i ] = name[ i ];
        }
        caName = caNameStorage;
    }
}
