/*******************************************************************************
 * @file	messageSend.c
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "messageSend.h"
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
uint8_t sendJ1939MessageToCANDriver( j1939Message_t message, canDriver_t driver )
{
    ( void ) message;
    uint8_t status = CAN_DRIVER_NOT_OPERATIONAL;
    if ( isCANDriverOperational( driver ) )
    {
        
    }

    return( status );
}