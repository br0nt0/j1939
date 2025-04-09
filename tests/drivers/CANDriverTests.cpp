/*******************************************************************************
 * @file	CANDriverTests.cpp
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C"
{
#include "drivers/CANDriver.h"
}

TEST_GROUP( CANDriver )
{
    CANMessage_t message;
    uint8_t data[ 3 ] = { 1u, 2u, 3u };
    void setup( void )
    {
        
    }
    void teardown( void )
    {
        destroyCANMessage( message );
    }
};

TEST( CANDriver, given_null_driver_when_destroying_CAN_driver_then_no_seg_fault )
{
    // given

    // when
    destroyCANDriver( NULL );

    // then
}

TEST( CANDriver, given_null_driver_when_sending_a_messge_then_no_seg_fault )
{
    // given
    message = createCANMessage( 0x123456u, true, data, 3u );

    // when
    uint8_t status = sendCANMessage( NULL, message );

    // then
    UNSIGNED_LONGS_EQUAL( CAN_DRIVER_IS_NULL, status );
}


TEST( CANDriver, given_a_test_CAN_driver_when_requesting_interface_type_then_it_is_returned )
{
    // given
    canDriverStruct_t testDriver;
    testDriver.type = "This is a type";

    // when
    
    // then
    STRCMP_EQUAL( "This is a type", getCANDriverType( &testDriver ) );
}

TEST( CANDriver, given_null_driver_when_checking_if_CAN_driver_is_operational_then_false_returned )
{
    // given

    // when

    // then
    CHECK_FALSE( isCANDriverOperational( NULL ) );
}

TEST( CANDriver, given_null_driver_when_receiving_a_message_then_no_seg_fault )
{
    // given

    // when
    message = receiveCANMessage( NULL );

    // then
    CHECK_TRUE( NULL == message );
}

TEST( CANDriver, given_null_driver_when_checking_for_buss_off_state_then_no_seg_fault )
{
    // given

    // when

    // then
    CHECK_FALSE( isCANTxBusOffState( NULL ) );
    
}