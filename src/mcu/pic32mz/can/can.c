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
    self->moduleRegs->canFifoRegisters[ fifo ].CxFIFOCONSET |= CXFIFOCON_UINC_MASK;
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

static void loadMessageIntoBuffer( const canMessage_t message, canTxMessageBuffer_t* buffer )
{
    if ( message->isExtended )
    {
        buffer->sid.SID = ( message->id >> 18u ) & 0x7ffu;
        buffer->eid.EID = message->id & 0x3ffffu;
        buffer->eid.IDE = 1u;
        buffer->eid.SRR = 1u;
    }
    else
    {
        buffer->sid.SID = message->id & 0x7ffu;
        buffer->eid.IDE = 0u;
    }
    buffer->eid.DLC = message->dlc;
    buffer->eid.RTR = 0u;
    buffer->eid.RB0 = 0u;
    buffer->eid.RB1 = 0u;
    for ( uint8_t i = 0u; i < message->dlc; ++i )
    {
        buffer->data[ i ] = message->data[ i ];
    }
}
static void flushTxFifo( picCANDriver_t driver, canFifo_t fifo )
{
    driver->moduleRegs->canFifoRegisters[ fifo ].CxFIFOCONSET |= CXFIFOCON_TXREQ_MASK;
}

static bool_t isTxFifoFull( picCANDriver_t driver, canFifo_t fifo )
{
    return ( driver->moduleRegs->canFifoRegisters[ fifo ].CxFIFOINT.bits.TXNFULLIF > 0u );
}

static bool_t wasErrorDetectedDuringTransmission( picCANDriver_t driver, canFifo_t fifo )
{
    return ( driver->moduleRegs->canFifoRegisters[ fifo ].CxFIFOCON.bits.TXERR > 0u );
}

static bool_t wasMessageAborted( picCANDriver_t driver, canFifo_t fifo )
{
    return ( driver->moduleRegs->canFifoRegisters[ fifo ].CxFIFOCON.bits.TXABAT > 0u );
}

static bool_t wasMessageArbitrationLost( picCANDriver_t driver, canFifo_t fifo )
{
    return ( driver->moduleRegs->canFifoRegisters[ fifo ].CxFIFOCON.bits.TXLARB > 0u );
}

static uint8_t getTxStatusByReadingErrorFlags( picCANDriver_t self, canFifo_t fifo )
{
    uint8_t status;
    if ( wasErrorDetectedDuringTransmission( self, fifo ) )
    {
        status = CAN_TX_ERROR_DETECTED;
    }
    else if ( wasMessageAborted( self, fifo ) )
    {
        status = CAN_TX_MESSAGE_ABORTED;
    }
    else if ( wasMessageArbitrationLost( self, fifo ) )
    {
        status = CAN_TX_MESSAGE_ARBITRATION_LOST;
    }
    else
    {
        status = CAN_TX_SUCCEEDED;
    }
    return ( status );
}

static uint8_t sendMessage( canDriver_t base, const canMessage_t message )
{
    picCANDriver_t self = ( picCANDriver_t ) base;
    uint8_t status = CAN_TX_BUFFER_FULL;
    if ( isTxFifoFull( self, CAN_FIFO1 ) )
    {
        canTxMessageBuffer_t* fifoBuffer = getTxCxFIFOUA( self->module, CAN_FIFO1 );
        loadMessageIntoBuffer( message, fifoBuffer );
        updateFifo( self, CAN_FIFO1 );
        flushTxFifo( self, CAN_FIFO1 );
        status = getTxStatusByReadingErrorFlags( self, CAN_FIFO1 );
    }

    return ( status );
}

/******************************************************************************/
canDriver_t createPIC32MZCANDriverForModule( uint8_t module )
{
    picCANDriver_t self = ( picCANDriver_t ) malloc( sizeof( struct picCANDriverStruct ) );
    self->interface.destroy = destroy;
    self->interface.isOperational = isOperational;
    self->interface.isTxBussOffState = isTxBusOff;
    self->interface.receiveMessage = receiveMessage;
    self->interface.sendMessage = sendMessage;
    self->base.vTable = &self->interface;
    self->base.type = "PIC32MZ";
    self->moduleRegs = getCANModuleRegisters( module );
    self->module = module;

    return ( ( canDriver_t ) self );
}

