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

TEST( CANMessage, given_an_ID_equal_to_0x7ff_when_creating_an_extended_CAN_message_then_it_is_extended )
{
    // given

    // when
    message = createExtendedCANMessage( 0x7ffu, data, dlc );

    // then
    CHECK_TRUE( isCANMessageExtended( message ) );
}

TEST( CANMessage, given_a_zero_size_for_data_when_creating_an_extended_CAN_message_then_null_returned )
{
    // given

    // when
    message = createExtendedCANMessage( id, data, 0u );

    // then
    CHECK_TRUE( message == NULL );
}

TEST( CANMessage, given_null_data_when_creating_an_extended_CAN_message_then_null_returned )
{
    // given

    // when
    message = createExtendedCANMessage( id, NULL, dlc );

    // then
    CHECK_TRUE( message == NULL );
}

TEST( CANMessage, given_valid_input_data_when_creating_an_extended_CAN_message_then_accessors_give_requested_information )
{
    // given

    // when
    message = createExtendedCANMessage( id, data, dlc );

    // then
    CHECK_FALSE( message == NULL );
    UNSIGNED_LONGS_EQUAL( id, getCANMessageID( message ) );
    CHECK_TRUE( isCANMessageExtended( message ) );
    MEMCMP_EQUAL( data, getCANMessageData( message ), getCANMessageDLC( message ) );
}

TEST( CANMessage, given_an_ID_bigger_than_0x7ff_when_creating_a_standard_CAN_message_then_null_returned )
{
    // given

    // when
    message = createStandardCANMessage( 0x7ffu + 1u, data, dlc );

    // then
    CHECK_TRUE( NULL == message );
}

TEST( CANMessage, given_a_zero_size_for_data_when_creating_a_standard_CAN_message_then_null_returned )
{
    // given

    // when
    message = createStandardCANMessage( id, data, 0u );

    // then
    CHECK_TRUE( message == NULL );
}

TEST( CANMessage, given_null_data_when_creating_a_standard_CAN_message_then_null_returned )
{
    // given

    // when
    message = createStandardCANMessage( id, NULL, dlc );

    // then
    CHECK_TRUE( message == NULL );
}

