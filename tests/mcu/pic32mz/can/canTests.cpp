/*******************************************************************************
 * @file	canTests.cpp
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C"
{
#include "mcu/pic32mz/can/can.h"
#include "mcu/pic32mz/can/canRegisterIO.h"
#include "string.h"
#include "mcu/pic32mz/can/canRxTxStructures.h"
}


TEST_GROUP( can )
{
    canDriver_t driver;
    canRegisters_t canRegs;
    canTxMessageBuffer_t txMessageBuffer;
    CANMessage_t canMessage = NULL;
    uint8_t data[ 4 ] = { 0x11, 0x22, 0x33, 0x44 };
    void setup( void )
    {
        expectOneCallToGetCANRegistersOfModule( CAN1 );
        driver = createPIC32MZCANDriverForModule( CAN1 );
        memset( ( void* ) &canRegs, 0, sizeof( canRegisters_t ) );
    }
    void teardown( void )
    {
        destroyCANDriver( driver );
        destroyCANMessage( canMessage );
        mock( "PIC32MZ_CAN" ).checkExpectations( );
		mock( ).clear( );
    }
    void expectOneCallToGetCANRegistersOfModule( canModule_t module )
    {
        mock( "PIC32MZ_CAN" )
        .expectOneCall( "getPIC32MZCANModuleRegs" )
        .withParameter( "module", module )
        .andReturnValue( &canRegs );
    }
    void expectOneCallToGetRxFifoUserAddressRegisterOfModule( canModule_t module, canFifo_t fifo, canRxMessageBuffer_t* buffer )
    {
        mock( "PIC32MZ_CAN" )
            .expectOneCall( "getPIC32MZRxCxFIFOUA" )
            .withParameter( "module", module )
            .withParameter( "fifo", fifo )
            .andReturnValue( buffer );
    }
    void expectOneCallToGetTxFifoUserAddressRegisterOfModule( canModule_t module, canFifo_t fifo, canTxMessageBuffer_t* buffer )
    {
        mock( "PIC32MZ_CAN" )
            .expectOneCall( "getPIC32MZTxCxFIFOUA" )
            .withParameter( "module", module )
            .withParameter( "fifo", fifo )
            .andReturnValue( buffer );
    }
    void setUpExtendedTransmittedMessage( )
    {
        canMessage = createExtendedCANMessage( 0x1fffffffu, data, 4u );

        expectOneCallToGetTxFifoUserAddressRegisterOfModule( CAN1, CAN_FIFO1, &txMessageBuffer );
    }
};

TEST( can, given_a_pic32mz_can_driver_then_it_can_be_created_and_destroyed )
{
    // given
    
    // when

    // then
    STRCMP_EQUAL( "PIC32MZ", getCANDriverType( driver ) );
}

TEST( can, given_CAN2_when_creating_a_pic32mz_CAN_dirver_then_CAN_module_2_is_configured )
{
    // given
    expectOneCallToGetCANRegistersOfModule( CAN2 );
    canDriver_t can2Driver = createPIC32MZCANDriverForModule( CAN2 );

    // when

    // then
    destroyCANDriver( can2Driver );
}

TEST( can, given_operational_module_when_checking_the_status_then_true_status_is_returned )
{
    // given
    canRegs.CxCON.bits.CANBUSY = 1u;

    // when

    // then
    CHECK_TRUE( isCANDriverOperational( driver ) );
}

TEST( can, given_non_operational_module_when_checking_the_status_then_false_status_is_returned )
{
    // given
    canRegs.CxCON.bits.CANBUSY = 0u;

    // when

    // then
    CHECK_FALSE( isCANDriverOperational( driver ) );
}

TEST( can, given_transmit_bus_off_when_checking_the_status_then_true_status_is_returned )
{
    // given
    canRegs.CxTREC = CAN_TX_BUS_OFF_STATE << 16u;

    // when

    // then
    CHECK_TRUE( isCANTxBusOffState( driver ) );
}

TEST( can, given_no_transmit_bus_off_when_checking_the_status_then_false_status_is_returned )
{
    // given
    canRegs.CxTREC = 0u;

    // when

    // then
    CHECK_FALSE( isCANTxBusOffState( driver ) );
}

TEST( can, given_an_empty_receive_fifo_when_receiving_a_CAN_message_then_null_is_returned )
{
    // given
    canRegs.canFifoRegisters[ CAN_FIFO0 ].CxFIFOINT.bits.RXNEMPTYIF = 0u;

    // when
    canMessage = receiveCANMessage( driver );

    // then
    POINTERS_EQUAL( NULL, canMessage );
}

TEST( can, given_a_non_empty_receive_fifo_and_a_null_user_address_register_when_receiving_a_CAN_message_then_null_is_returned )
{
    // given
    canRegs.canFifoRegisters[ CAN_FIFO0 ].CxFIFOINT.bits.RXNEMPTYIF = 1u;
    expectOneCallToGetRxFifoUserAddressRegisterOfModule( CAN1, CAN_FIFO0, NULL );

    // when
    canMessage = receiveCANMessage( driver );

    // then
    POINTERS_EQUAL( NULL, canMessage );
}

TEST( can, given_fifo_0_with_one_message_when_receiving_a_CAN_message_then_an_extended_frame_can_be_received )
{
    // given
    canRxMessageBuffer_t msg;
    msg.eid.EID = 0xbabau;
	msg.sid.SID = 0xc0u;
	msg.eid.DLC = 8u;
    msg.eid.IDE = 1u;
    expectOneCallToGetRxFifoUserAddressRegisterOfModule( CAN1, CAN_FIFO0, &msg );
    canRegs.canFifoRegisters[ CAN_FIFO0 ].CxFIFOINT.bits.RXNEMPTYIF = 1u;

    // when
    canMessage = receiveCANMessage( driver );

    // then
    UNSIGNED_LONGS_EQUAL( msg.eid.EID | ( msg.sid.SID << 18u ), getCANMessageID( canMessage ) );
    UNSIGNED_LONGS_EQUAL( msg.eid.DLC, getCANMessageDLC( canMessage ) );
    CHECK_TRUE( isCANMessageExtended( canMessage ) );
}


TEST( can, given_fifo_0_with_one_message_when_receiving_a_CAN_message_then_a_standard_frame_can_be_received )
{
    // given
    canRxMessageBuffer_t msg;
    msg.eid.EID = 0xbabau;
	msg.sid.SID = 0xc0u;
	msg.eid.DLC = 8u;
    msg.eid.IDE = 0u;
    expectOneCallToGetRxFifoUserAddressRegisterOfModule( CAN1, CAN_FIFO0, &msg );
    canRegs.canFifoRegisters[ CAN_FIFO0 ].CxFIFOINT.bits.RXNEMPTYIF = 1u;

    // when
    canMessage = receiveCANMessage( driver );

    // then
    UNSIGNED_LONGS_EQUAL( msg.sid.SID, getCANMessageID( canMessage ) );
    UNSIGNED_LONGS_EQUAL( msg.eid.DLC, getCANMessageDLC( canMessage ) );
    CHECK_FALSE( isCANMessageExtended( canMessage ) );
}

TEST( can, given_fifo_0_with_message_with_data_when_receiving_a_CAN_message_then_data_can_be_read )
{
    // given
    canRxMessageBuffer_t msg;
    msg.eid.DLC = 8u;
    memset( msg.data, 0xa3, msg.eid.DLC );
    expectOneCallToGetRxFifoUserAddressRegisterOfModule( CAN1, CAN_FIFO0, &msg );
    canRegs.canFifoRegisters[ CAN_FIFO0 ].CxFIFOINT.bits.RXNEMPTYIF = 1u;

    // when
    canMessage = receiveCANMessage( driver );

    // then
    MEMCMP_EQUAL( msg.data, getCANMessageData( canMessage ), getCANMessageDLC( canMessage ) );
}

TEST( can, given_fifo_0_as_receive_fifo_when_receiving_a_CAN_message_then_UINC_is_set )
{
    // given
    canRxMessageBuffer_t msg;
    expectOneCallToGetRxFifoUserAddressRegisterOfModule( CAN1, CAN_FIFO0, &msg );
    canRegs.canFifoRegisters[ CAN_FIFO0 ].CxFIFOINT.bits.RXNEMPTYIF = 1u;

    // when
    canMessage = receiveCANMessage( driver );

    // then
    UNSIGNED_LONGS_EQUAL( CXFIFOCON_UINC_MASK, canRegs.canFifoRegisters[ CAN_FIFO0 ].CxFIFOCONSET );
}

TEST( can, given_fifo_1_as_transmit_fifo_when_sending_an_extended_CAN_message_to_HW_then_registers_are_loaded_accordingly )
{
    // given
    setUpExtendedTransmittedMessage( );
    canRegs.canFifoRegisters[ CAN_FIFO1 ].CxFIFOINT.bits.TXNFULLIF = 1u;

    // when
    uint8_t status = sendCANMessage( driver, canMessage );

    // then
    UNSIGNED_LONGS_EQUAL( getCANMessageID( canMessage ) >> 18u & 0x7ffu, txMessageBuffer.sid.SID );
    UNSIGNED_LONGS_EQUAL( getCANMessageID( canMessage ) & 0x3ffffu, txMessageBuffer.eid.EID );
	UNSIGNED_LONGS_EQUAL( getCANMessageDLC( canMessage ), txMessageBuffer.eid.DLC );
	UNSIGNED_LONGS_EQUAL( 1u, txMessageBuffer.eid.SRR );
	UNSIGNED_LONGS_EQUAL( 1u, txMessageBuffer.eid.IDE );
	UNSIGNED_LONGS_EQUAL( 0u, txMessageBuffer.eid.RTR );
	UNSIGNED_LONGS_EQUAL( 0u, txMessageBuffer.eid.RB0 );
    UNSIGNED_LONGS_EQUAL( 0u, txMessageBuffer.eid.RB1 );
    MEMCMP_EQUAL( getCANMessageData( canMessage ), txMessageBuffer.data, txMessageBuffer.eid.DLC );
    UNSIGNED_LONGS_EQUAL( CAN_TX_SUCCEEDED, status );
}

TEST( can, given_fifo_1_as_transmit_fifo_when_sending_an_standard_CAN_message_to_HW_then_registers_are_loaded_accordingly )
{
    // given
    canMessage = createStandardCANMessage( 0x7ffu, data, 4u );

    expectOneCallToGetTxFifoUserAddressRegisterOfModule( CAN1, CAN_FIFO1, &txMessageBuffer );
    canRegs.canFifoRegisters[ CAN_FIFO1 ].CxFIFOINT.bits.TXNFULLIF = 1u;

    // when
    uint8_t status = sendCANMessage( driver, canMessage );

    // then
    UNSIGNED_LONGS_EQUAL( getCANMessageID( canMessage ) & 0x7ffu, txMessageBuffer.sid.SID );
	UNSIGNED_LONGS_EQUAL( getCANMessageDLC( canMessage ), txMessageBuffer.eid.DLC );
	UNSIGNED_LONGS_EQUAL( 0u, txMessageBuffer.eid.SRR );
	UNSIGNED_LONGS_EQUAL( 0u, txMessageBuffer.eid.IDE );
	UNSIGNED_LONGS_EQUAL( 0u, txMessageBuffer.eid.RTR );
	UNSIGNED_LONGS_EQUAL( 0u, txMessageBuffer.eid.RB0 );
    UNSIGNED_LONGS_EQUAL( 0u, txMessageBuffer.eid.RB1 );
    MEMCMP_EQUAL( getCANMessageData( canMessage ), txMessageBuffer.data, txMessageBuffer.eid.DLC );
    UNSIGNED_LONGS_EQUAL( CAN_TX_SUCCEEDED, status );
}

TEST( can, given_fifo_1_as_transmit_fifo_when_sending_a_CAN_message_to_HW_then_transmit_fifo_is_flushed )
{
    // given
    canMessage = createStandardCANMessage( 0x7ffu, data, 4u );

    expectOneCallToGetTxFifoUserAddressRegisterOfModule( CAN1, CAN_FIFO1, &txMessageBuffer );
    canRegs.canFifoRegisters[ CAN_FIFO1 ].CxFIFOINT.bits.TXNFULLIF = 1u;

    // when
    uint8_t status = sendCANMessage( driver, canMessage );

    // then
    UNSIGNED_LONGS_EQUAL( CAN_TX_SUCCEEDED, status );
    CHECK_TRUE( ( canRegs.canFifoRegisters[ CAN_FIFO1 ].CxFIFOCONSET & CXFIFOCON_TXREQ_MASK ) > 0u );
}

TEST( can, given_full_transmit_fifo_when_sending_a_CAN_message_to_HW_then_no_message_is_sent_and_status_is_buffer_full )
{
    // given
    txMessageBuffer.sid.SID = 0x123u;
    canMessage = createStandardCANMessage( 0x7ffu, data, 4u );

    mock( "PIC32MZ_CAN" ).expectNoCall( "getPIC32MZRxCxFIFOUA" );
    canRegs.canFifoRegisters[ CAN_FIFO1 ].CxFIFOINT.bits.TXNFULLIF = 0u;

    // when
    uint8_t status = sendCANMessage( driver, canMessage );

    // then
    UNSIGNED_LONGS_EQUAL( CAN_TX_BUFFER_FULL, status );
    UNSIGNED_LONGS_EQUAL( 0x123u, txMessageBuffer.sid.SID );
}

TEST( can, given_a_transmit_fifo_when_sending_a_CAN_message_to_HW_then_UINC_register_is_set )
{
    // given
    setUpExtendedTransmittedMessage( );
    canRegs.canFifoRegisters[ CAN_FIFO1 ].CxFIFOINT.bits.TXNFULLIF = 1u;

    // when
    uint8_t status = sendCANMessage( driver, canMessage );

    // then
    UNSIGNED_LONGS_EQUAL( CAN_TX_SUCCEEDED, status );
    CHECK_TRUE( ( canRegs.canFifoRegisters[ CAN_FIFO1 ].CxFIFOCONSET & CXFIFOCON_UINC_MASK ) > 0u );
}


TEST( can, given_a_transmission_error_when_sending_a_CAN_message_to_HW_then_status__error_detected_returned )
{
    // given
    setUpExtendedTransmittedMessage( );
    canRegs.canFifoRegisters[ CAN_FIFO1 ].CxFIFOINT.bits.TXNFULLIF = 1u;
    canRegs.canFifoRegisters[ CAN_FIFO1 ].CxFIFOCON.bits.TXERR = 1u;

    // when
    uint8_t status = sendCANMessage( driver, canMessage );

    // then
    UNSIGNED_LONGS_EQUAL( CAN_TX_ERROR_DETECTED, status );
}

TEST( can, given_a_transmission_message_abort_when_sending_a_CAN_message_to_HW_then_status_message_abort_returned )
{
    // given
    setUpExtendedTransmittedMessage( );
    canRegs.canFifoRegisters[ CAN_FIFO1 ].CxFIFOINT.bits.TXNFULLIF = 1u;
    canRegs.canFifoRegisters[ CAN_FIFO1 ].CxFIFOCON.bits.TXABAT = 1u;

    // when
    uint8_t status = sendCANMessage( driver, canMessage );

    // then
    UNSIGNED_LONGS_EQUAL( CAN_TX_MESSAGE_ABORTED, status );
}

TEST( can, given_a_transmission_message_arbitration_lost_when_sending_a_CAN_message_to_HW_then_status_arbitration_lost_returned )
{
    // given
    setUpExtendedTransmittedMessage( );
    canRegs.canFifoRegisters[ CAN_FIFO1 ].CxFIFOINT.bits.TXNFULLIF = 1u;
    canRegs.canFifoRegisters[ CAN_FIFO1 ].CxFIFOCON.bits.TXLARB = 1u;

    // when
    uint8_t status = sendCANMessage( driver, canMessage );

    // then
    UNSIGNED_LONGS_EQUAL( CAN_TX_MESSAGE_ARBITRATION_LOST, status );
}

