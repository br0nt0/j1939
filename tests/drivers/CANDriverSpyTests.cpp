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

