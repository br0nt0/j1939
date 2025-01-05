/*******************************************************************************
 * @file	can.c
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "can.h"
#include "commonTypes.h"
#include "canRegisterIO.h"
#include "canRxTxStructures.h"

/******************************************************************************/
struct picCANDriverStruct
{
    canDriverStruct_t base;
    canDriverInterfaceStruct_t interface;
    canRegisters_t* moduleRegs;
    canMessageStruct_t rxMessage;
    canModule_t module;
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
    return ( self->moduleRegs->CxCON.bits.CANBUSY > 0u );
}

static bool_t isTxBusOff( canDriver_t base )
{
    picCANDriver_t self = ( picCANDriver_t ) base;
    return ( ( ( self->moduleRegs->CxTREC >> 16u ) & CAN_TX_BUS_OFF_STATE ) > 0u );
}

static void processMessageBuffer( canMessage_t msg, canRxMessageBuffer_t* buffer )
{
	msg->isExtended = ( buffer->eid.IDE > 0u ) ? true : false;
	if ( msg->isExtended )
	{
		msg->id = ( uint32_t ) buffer->eid.EID + ( ( uint32_t ) buffer->sid.SID << 18u );
	}
	else
	{
		msg->id = buffer->sid.SID;
	}
	msg->dlc = ( uint8_t ) buffer->eid.DLC;
	msg->data = buffer->data;
}

static void updateFifo( picCANDriver_t self, canFifo_t fifo )
{
    self->moduleRegs->canFifoRegisters[ fifo ].CxFIFOCONSET = CXFIFOCON_UINC_MASK;
}

static bool_t isReceiveFifoEmpty( picCANDriver_t self, uint8_t fifo )
{
    return ( self->moduleRegs->canFifoRegisters[ fifo ].CxFIFOINT.bits.RXNEMPTYIF > 0u );
}

static canMessage_t receiveMessage( canDriver_t base )
{
    picCANDriver_t self = ( picCANDriver_t ) base;
    canMessage_t msg = NULL;
    if ( isReceiveFifoEmpty( self, CAN_FIFO0 ) )
    {
        canRxMessageBuffer_t* buffer = getRxCxFIFOUA( self->module, CAN_FIFO0 );
        if ( NULL != buffer )
        {
            msg = &self->rxMessage;
            processMessageBuffer( msg, buffer );
            updateFifo( self, CAN_FIFO0 );
        }
    }

    return ( msg );
}

/******************************************************************************/
canDriver_t createPIC32MZCANDriverForModule( uint8_t module )
{
    picCANDriver_t self = ( picCANDriver_t ) malloc( sizeof( struct picCANDriverStruct ) );
    self->interface.destroy = destroy;
    self->interface.isOperational = isOperational;
    self->interface.isTxBussOffState = isTxBusOff;
    self->interface.receiveMessage = receiveMessage;
    self->base.vTable = &self->interface;
    self->base.type = "PIC32MZ";
    self->moduleRegs = getCANModuleRegisters( module );
    self->module = module;

    return ( ( canDriver_t ) self );
}

