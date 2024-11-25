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
    canMessageStruct_t message;
    message.id = 0x1cfd5411u;
    message.dlc = 8u;
    message.isExtended = true;
    message.data = data;
    mock( "CANSpy" ).expectOneCall( "sendMessage" )
        .withPointerParameter( "base", spy )
        .withUnsignedIntParameter( "id", message.id )
        .withBoolParameter( "isExtended", message.isExtended )
        .withUnsignedIntParameter( "dlc", message.dlc )
        .withPointerParameter( "data", message.data )
        .andReturnValue( CAN_TX_SUCCEEDED );

    // when
    uint8_t result = sendCANMessage( spy, &message );

    // then
    UNSIGNED_LONGS_EQUAL( result, CAN_TX_SUCCEEDED );
}
