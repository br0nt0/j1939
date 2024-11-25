/*******************************************************************************
 * @file	messageSendTests.cpp
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "drivers/CANDriverSpy.h"

extern "C"
{
#include "j1939/messageSend.h"
}

TEST_GROUP( messageSend )
{
    canDriver_t spy;
    j1939MessageStruct_t message;
    void setup( void )
    {
        spy = createCANDriverSpy( );
    }
    void teardown( void )
    {
        destroyCANDriver( spy );
        mock( "CANSpy" ).checkExpectations( );
		mock( ).clear( );
    }
};

TEST( messageSend, given_a_non_operational_can_driver_when_sending_j1939_message_to_CAN_layer_then_status_is_can_driver_is_not_operational )
{
    // given
    mock( "CANSpy" ).expectOneCall( "isOperational" ).withPointerParameter( "base", spy ).andReturnValue( false );

    // when
    uint8_t status = sendJ1939MessageToCANDriver( &message, spy );

    // then
    UNSIGNED_LONGS_EQUAL( CAN_DRIVER_NOT_OPERATIONAL, status );
}

TEST( messageSend, given_an_operational_can_driver_when_sending_j1939_message_to_CAN_layer_then_message_is_received_by_can_driver_layer )
{
    // given
    uint8_t data[ 8 ] = { 1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u };
    message.parameterGroupNumber = 0xfe43u;
    message.priority = 6u;
    message.sourceAddress = 0x12u;
    message.destinationAddress = 0x13u;
    message.dataSize = 8u;
    message.data = data;

    canMessageStruct_t expectedCANMessage;
    expectedCANMessage.id = ( message.priority << 26u ) + ( message.parameterGroupNumber << 8u ) + ( message.sourceAddress );
    expectedCANMessage.isExtended = true;
    expectedCANMessage.dlc = ( uint8_t ) message.dataSize;
    expectedCANMessage.data = message.data;

    mock( "CANSpy" ).expectOneCall( "isOperational" )
        .withPointerParameter( "base", spy )
        .andReturnValue( true );
    mock( "CANSpy" ).expectOneCall( "sendMessage" )
        .withPointerParameter( "base", spy )
        .withUnsignedIntParameter( "id", expectedCANMessage.id )
        .withBoolParameter( "isExtended", expectedCANMessage.isExtended )
        .withUnsignedIntParameter( "dlc", expectedCANMessage.dlc )
        .withPointerParameter( "data", expectedCANMessage.data )
        .andReturnValue( CAN_TX_SUCCEEDED );

    // when
    uint8_t status = sendJ1939MessageToCANDriver( &message, spy );

    // then
    UNSIGNED_LONGS_EQUAL( CAN_TX_SUCCEEDED, status );
}




