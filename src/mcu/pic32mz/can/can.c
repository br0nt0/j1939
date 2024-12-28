/*******************************************************************************
 * @file	can.c
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "can.h"
#include "commonTypes.h"

/******************************************************************************/
typedef struct picCANDriverStruct* picCANDriver_t;
struct picCANDriverStruct
{
    canDriverStruct_t base;
    canDriverInterfaceStruct_t interface;
};
/******************************************************************************/
static void destroy( canDriver_t base )
{
    picCANDriver_t self = ( picCANDriver_t ) base;
    free( self );
}

static bool_t isOperational( canDriver_t base )
{
    picCANDriver_t self = ( picCANDriver_t ) base;
    ( void ) self;
    return ( true );
}

static uint8_t sendMessage( canDriver_t base, const canMessage_t message )
{
    ( void ) base;
    ( void ) message;
    return( 0u );
}

static canMessage_t receiveMessage( canDriver_t base )
{
    ( void ) base;
    return ( NULL );
}

static bool_t isTxBusOffState( canDriver_t base )
{
    ( void ) base;
    return ( false );
}

/******************************************************************************/
canDriver_t createPIC32MZCANDriver( void )
{
    picCANDriver_t self = ( picCANDriver_t ) malloc( sizeof( struct picCANDriverStruct ) );
    self->interface.destroy = destroy;
    self->interface.isOperational = isOperational;
    self->interface.isTxBussOffState = isTxBusOffState;
    self->interface.sendMessage = sendMessage;
    self->interface.receiveMessage = receiveMessage;
    self->base.vTable = &self->interface;
    self->base.type = "PIC32MZ";

    return ( ( canDriver_t ) self );
}
