/*******************************************************************************
 * @file	CANDriverSpyTests.cpp
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "CANDriverSpy.h"

TEST_GROUP( CANDriverSpy )
{
    canDriver_t spy;
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

TEST( CANDriverSpy, given_spy_CAN_driver_then_create_and_destroy )
{
    // given

    // when

    // then
}

TEST( CANDriverSpy, given_spy_CAN_driver_then_type_is_returned )
{
    // given

    // when

    // then
    STRCMP_EQUAL( "SpyCANDriver", getCANDriverType( spy ) );
}

TEST( CANDriverSpy, given_spy_CAN_driver_when_checking_if_operational_then_value_is_returned )
{
    // given
    mock( "CANSpy" ).expectOneCall( "isOperational" ).withPointerParameter( "base", spy ).andReturnValue( true );

    // when

    // then
    CHECK_TRUE( isCANDriverOperational( spy ) );
}

TEST( CANDriverSpy, given_spy_CAN_driver_when_transmitting_a_message_then_the_expected_message_is_returned )
{
    // given
    uint8_t data[ 8 ] = { 7u, 6u, 5u, 4u, 3u, 2u, 1u, 0u };
    CANMessage_t message = createCANMessage( 0x1cfd5411u, true, data, 8u );
    mock( "CANSpy" ).expectOneCall( "sendMessage" )
        .withPointerParameter( "base", spy )
        .withUnsignedIntParameter( "id", getCANMessageID( message ) )
        .withBoolParameter( "isExtended", isCANMessageExtended( message ) )
        .withUnsignedIntParameter( "dlc", getCANMessageDLC( message ) )
        .withPointerParameter( "data", getCANMessageData( message ) )
        .andReturnValue( CAN_TX_SUCCEEDED );

    // when
    uint8_t result = sendCANMessage( spy, message );

    // then
    UNSIGNED_LONGS_EQUAL( result, CAN_TX_SUCCEEDED );
    destroyCANMessage( message );
}

TEST( CANDriverSpy, given_spy_CAN_driver_when_receiving_one_message_then_the_expected_message_is_returned )
{
    // given
    uint8_t data[ 3 ] = { 3u, 4u, 5u };
    CANMessage_t expectedMessage = createCANMessage( 0x423432u, true, data, 3u );
    mock( "CANSpy" ).expectOneCall( "receiveMessage" )
        .withPointerParameter( "base", spy )
        .andReturnValue( expectedMessage );

    // when
    CANMessage_t actualMessage = receiveCANMessage( spy );

    // then
    POINTERS_EQUAL( expectedMessage, actualMessage );
    destroyCANMessage( actualMessage );
}

TEST( CANDriverSpy, given_spy_CAN_driver_when_receiving_many_messages_then_the_expected_messages_are_returned )
{
    // given
    uint8_t data[ 3 ] = { 1u,2u,3u };
    CANMessage_t expectedMessage1 = createCANMessage( 0x21345u, true, data, 3u );
    CANMessage_t expectedMessage2 = createCANMessage( 0x543253u, true, data, 3u );
    mock( "CANSpy" ).expectOneCall( "receiveMessage" )
        .withPointerParameter( "base", spy )
        .andReturnValue( expectedMessage1 );
    mock( "CANSpy" ).expectOneCall( "receiveMessage" )
        .withPointerParameter( "base", spy )
        .andReturnValue( expectedMessage2 );
    
    // when
    CANMessage_t actualMessage1 = receiveCANMessage( spy );
    CANMessage_t actualMessage2 = receiveCANMessage( spy );

    // then
    POINTERS_EQUAL( expectedMessage1, actualMessage1 );
    POINTERS_EQUAL( expectedMessage2, actualMessage2 );
    destroyCANMessage( actualMessage1 );
    destroyCANMessage( actualMessage2 );
}

TEST( CANDriverSpy, given_spy_CAN_driver_when_checking_if_buss_off_state_then_value_is_returned )
{
    // given
    mock( "CANSpy" ).expectOneCall( "isTxBusOffState" ).withPointerParameter( "base", spy ).andReturnValue( true );

    // when

    // then
    CHECK_TRUE( isCANTxBusOffState( spy ) );
}