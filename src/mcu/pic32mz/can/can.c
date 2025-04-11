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
    canDriverStruct_t super;
    canDriverInterfaceStruct_t interface;
    canRegisters_t* moduleRegs;
    canModule_t module;
};

typedef struct picCANDriverStruct* picCANDriver_t;

/******************************************************************************/
static void destroy( canDriver_t super )
{
    picCANDriver_t self = ( picCANDriver_t ) super;
    free( self );
}

static bool_t isOperational( canDriver_t super )
{
    picCANDriver_t self = ( picCANDriver_t ) super;
    return ( self->moduleRegs->CxCON.bits.CANBUSY > 0u );
}

static bool_t isTxBusOff( canDriver_t super )
{
    picCANDriver_t self = ( picCANDriver_t ) super;
    return ( ( ( self->moduleRegs->CxTREC >> 16u ) & CAN_TX_BUS_OFF_STATE ) > 0u );
}

static void updateFifo( picCANDriver_t self, canFifo_t fifo )
{
    self->moduleRegs->canFifoRegisters[ fifo ].CxFIFOCONSET |= CXFIFOCON_UINC_MASK;
}

static bool_t isReceiveFifoEmpty( picCANDriver_t self, uint8_t fifo )
{
    return ( self->moduleRegs->canFifoRegisters[ fifo ].CxFIFOINT.bits.RXNEMPTYIF > 0u );
}

static uint32_t getIDFromBuffer( canRxMessageBuffer_t* buffer, bool_t isExtended )
{
    uint32_t id;
    if ( isExtended )
    {
        id = ( uint32_t ) buffer->eid.EID + ( ( uint32_t ) buffer->sid.SID << 18u );
    }
    else
    {
        id = buffer->sid.SID;
    }
    return ( id );
}

static CANMessage_t receiveMessage( canDriver_t super )
{
    picCANDriver_t self = ( picCANDriver_t ) super;
    CANMessage_t msg = NULL;
    if ( isReceiveFifoEmpty( self, CAN_FIFO0 ) )
    {
        canRxMessageBuffer_t* buffer = getRxCxFIFOUA( self->module, CAN_FIFO0 );
        if ( NULL != buffer )
        {
            bool_t isExtended = ( buffer->eid.IDE > 0u ) ? true : false;
            uint32_t id = getIDFromBuffer( buffer, isExtended );
            if ( isExtended )
            {
                msg = createExtendedCANMessage( id, buffer->data, ( uint8_t ) buffer->eid.DLC );
            }
            else
            {
                msg = createStandardCANMessage( id, buffer->data, ( uint8_t ) buffer->eid.DLC );
            }

            updateFifo( self, CAN_FIFO0 );
        }
    }
    return ( msg );
}

static void loadMessageIntoBuffer( const CANMessage_t message, canTxMessageBuffer_t* buffer )
{
    uint32_t id = getCANMessageID( message );
    if ( isCANMessageExtended( message ) )
    {        
        buffer->sid.SID = ( id >> 18u ) & 0x7ffu;
        buffer->eid.EID = id & 0x3ffffu;
        buffer->eid.IDE = 1u;
        buffer->eid.SRR = 1u;
    }
    else
    {
        buffer->sid.SID = id & 0x7ffu;
        buffer->eid.IDE = 0u;
    }
    buffer->eid.DLC = getCANMessageDLC( message );
    buffer->eid.RTR = 0u;
    buffer->eid.RB0 = 0u;
    buffer->eid.RB1 = 0u;
    const uint8_t* data = getCANMessageData( message );
    for ( uint8_t i = 0u; i < getCANMessageDLC( message ); ++i )
    {
        buffer->data[ i ] = data[ i ];
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

static uint8_t sendMessage( canDriver_t super, const CANMessage_t message )
{
    picCANDriver_t self = ( picCANDriver_t ) super;
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
    self->super.vTable = &self->interface;
    self->super.type = "PIC32MZ";
    self->moduleRegs = getCANModuleRegisters( module );
    self->module = module;

    return ( ( canDriver_t ) self );
}

