/*******************************************************************************
 * @file	can.c
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "can.h"
#include "commonTypes.h"
#include "canRegisterIO.h"

/******************************************************************************/
struct picCANDriverStruct
{
    canDriverStruct_t base;
    canDriverInterfaceStruct_t interface;
    uint8_t module;
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
    canRegisters_t* registers = getCANModuleRegisters( self->module );
    return ( registers->CxCON.bits.CANBUSY > 0u );
}

/******************************************************************************/
canDriver_t createPIC32MZCANDriverForModule( uint8_t module )
{
    picCANDriver_t self = ( picCANDriver_t ) malloc( sizeof( struct picCANDriverStruct ) );
    self->interface.destroy = destroy;
    self->interface.isOperational = isOperational;
    self->base.vTable = &self->interface;
    self->base.type = "PIC32MZ";
    self->module = module;

    return ( ( canDriver_t ) self );
}

uint8_t getPIC32MZCANConfiguredModule( canDriver_t base )
{
    picCANDriver_t self = ( picCANDriver_t ) base;
    return ( self->module );
}