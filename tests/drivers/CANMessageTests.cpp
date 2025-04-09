/*******************************************************************************
 * @file	CANMessageTests.cpp
 * @brief	
 * @author	@br0nt0
 * @date	2025
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C"
{
#include "drivers/CANMessage.h"
}

TEST_GROUP( CANMessage )
{
    CANMessage_t message = NULL;
    uint32_t id = 0x8000u;
    uint8_t data[ 3 ] = { 1u, 2u, 3u };
    uint8_t dlc = 3u;
    bool_t isExtended = true;
    void setup( void )
    {
    }
    void teardown( void )
    {
        destroyCANMessage( message );
    }
};

TEST( CANMessage, given_a_null_CAN_message_when_destroying_it_then_nothing_happens )
{
    // given

    // when

    // then
}

TEST( CANMessage, given_a_null_CAN_message_when_checking_if_it_is_extended_then_false_returned )
{
    // given

    // when

    // then
    CHECK_FALSE( isCANMessageExtended( NULL ) );
}

TEST( CANMessage, given_a_null_CAN_message_when_getting_the_data_then_null_returned )
{
    // given

    // when

    // then
    CHECK_TRUE( NULL == getCANMessageData( NULL ) );
}

TEST( CANMessage, given_a_null_CAN_message_when_getting_the_dlc_then_null_returned )
{
    // given

    // when

    // then
    UNSIGNED_LONGS_EQUAL( 0u, getCANMessageDLC( NULL ) );
}

TEST( CANMessage, given_a_null_CAN_message_when_getting_then_ID_then_zero_returned )
{
    // given

    // when

    // then
    UNSIGNED_LONGS_EQUAL( 0u, getCANMessageID( NULL ) );
}

TEST( CANMessage, given_a_standard_CAN_message_when_creating_it_then_it_is_not_extended )
{
    // given

    // when
    message = createCANMessage( id, false, data, dlc );

    // then
    CHECK_FALSE( isCANMessageExtended( message ) );
}

TEST( CANMessage, given_a_CAN_message_with_zero_dlc_when_creating_it_then_null_returned )
{
    // given

    // when
    message = createCANMessage( id, isExtended, data, 0u );

    // then
    CHECK_TRUE( message == NULL );
}

TEST( CANMessage, given_a_CAN_message_with_null_data_when_creating_it_then_null_returned )
{
    // given

    // when
    message = createCANMessage( id, isExtended, NULL, dlc );

    // then
    CHECK_TRUE( message == NULL );
}

TEST( CANMessage, given_an_extended_CAN_message_when_creating_it_then_accessors_return_the_correct_values )
{
    // given

    // when
    message = createCANMessage( id, isExtended, data, dlc );

    // then
    CHECK_FALSE( message == NULL );
    UNSIGNED_LONGS_EQUAL( id, getCANMessageID( message ) );
    CHECK_TRUE( isCANMessageExtended( message ) );
    MEMCMP_EQUAL( data, getCANMessageData( message ), getCANMessageDLC( message ) );
}

