/*******************************************************************************
 * @file	j1939MessageTests.cpp
 * @brief	
 * @author	@br0nt0
 * @date	2025
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C"
{
#include "j1939/j1939Message.h"
#include "j1939/iAddressClaimed.h"
}

TEST_GROUP( j1939Message )
{
    j1939Message_t j1939Message;
    uint16_t parameterGroupNumber = 0x1234u;
    uint8_t data[ 8 ] = { 0x01u, 0x02u, 0x03u, 0x04u, 0x05u, 0x06u, 0x07u, 0x08u };
    uint8_t dataLength = 8u;
    uint8_t priority = 0x01u;
    uint8_t destinationAddress = 0x03u;
    uint8_t sourceAddress = 0x02u;
    void setup( void )
    {
        j1939Message = createJ1939Message( parameterGroupNumber, priority, destinationAddress, sourceAddress, data, dataLength );
    }
    void teardown( void )
    {
        destroyJ1939Message( j1939Message );
    }
};

TEST( j1939Message, given_a_j1939_message_then_it_can_be_created_and_then_destroyed )
{
    // given

    // when

    // then
}

TEST( j1939Message, given_a_null_input_to_destroy_function_then_nothing_happens )
{
    // given

    // when
    destroyJ1939Message( NULL );

    // then
}

TEST( j1939Message, given_a_zero_PGN_when_creating_a_J1939_message_then_returned_value_is_null )
{
    // given
    parameterGroupNumber = 0u;

    // when
    j1939Message_t nullMessage = createJ1939Message( parameterGroupNumber, priority, destinationAddress, sourceAddress, data, dataLength );

    // then
    CHECK_TRUE( nullMessage == NULL );
}

TEST( j1939Message, given_null_data_when_creating_a_J1939_message_then_returned_value_is_null )
{
    // given

    // when
    j1939Message_t nullMessage = createJ1939Message( parameterGroupNumber, priority, destinationAddress, sourceAddress, NULL, dataLength );

    // then
    CHECK_TRUE( nullMessage == NULL );
}

TEST( j1939Message, given_null_data_size_when_creating_a_J1939_message_then_returned_value_is_null )
{
    // given

    // when
    j1939Message_t nullMessage = createJ1939Message( parameterGroupNumber, priority, destinationAddress, sourceAddress, data, 0u );

    // then
    CHECK_TRUE( nullMessage == NULL );
}

TEST( j1939Message, given_pgn_priority_destination_address_and_data_when_creating_a_J1939_message_then_they_can_be_retrieved )
{
    // given

    // when

    // then
    UNSIGNED_LONGS_EQUAL( parameterGroupNumber, getPGN( j1939Message ) );
    UNSIGNED_LONGS_EQUAL( priority, getPriority( j1939Message ) );
    UNSIGNED_LONGS_EQUAL( destinationAddress, getDA( j1939Message ) );
    UNSIGNED_LONGS_EQUAL( sourceAddress, getSA( j1939Message ) );
    MEMCMP_EQUAL( data, getData( j1939Message ), dataLength );
}