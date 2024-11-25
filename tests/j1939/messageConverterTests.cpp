/*******************************************************************************
 * @file	messageConverterTests.cpp
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C"
{
#include "j1939/messageConverter.h"
}

TEST_GROUP( encodeMessage )
{
    canMessageStruct_t canMessage;
    j1939MessageStruct_t j1939Message;
    void setup( void )
    {
    }
    void teardown( void )
    {
    }
};

TEST( encodeMessage, given_null_inputs_to_the_converter_then_nothing_happens )
{
    // given

    // when
    encodeCANMessage( NULL, NULL );
    encodeCANMessage( &canMessage, NULL );
    encodeCANMessage( NULL, &j1939Message );

    // then
}

TEST( encodeMessage, given_a_j1939_message_with_address_bigger_than_zero_when_encoding_to_CAN_message_then_CAN_id_last_8_bits_has_the_address )
{
    // given
    j1939Message.sourceAddress = 0x01u;

    // when
    encodeCANMessage( &canMessage, &j1939Message );

    // then
    uint8_t sourceAddress = ( uint8_t ) ( canMessage.id );
    UNSIGNED_LONGS_EQUAL( j1939Message.sourceAddress, sourceAddress );
}

TEST( encodeMessage, given_a_j1939_message_with_priority_bigger_than_zero_when_encoding_to_CAN_message_then_CAN_id_has_it_on_bit_position_26 )
{
    // given
    j1939Message.priority = 0x01u;

    // when
    encodeCANMessage( &canMessage, &j1939Message );

    // then
    uint8_t priority = ( uint8_t ) ( canMessage.id >> 26u );
    UNSIGNED_LONGS_EQUAL( j1939Message.priority, priority );
}

TEST( encodeMessage, given_a_j1939_message_with_protocol_format_equal_to_240_when_encoding_to_CAN_message_then_PS_field_is_group_extension )
{
    // given
    j1939Message.parameterGroupNumber = 0xf000U;
    j1939Message.destinationAddress = 0x32u;

    // when
    encodeCANMessage( &canMessage, &j1939Message );

    // then
    uint32_t pgn = ( uint32_t ) ( canMessage.id >> 8u );
    UNSIGNED_LONGS_EQUAL( j1939Message.parameterGroupNumber, pgn );
}

TEST( encodeMessage, given_a_j1939_message_with_protocol_format_bigger_than_240_when_encoding_to_CAN_message_then_PS_field_is_group_extension )
{
    // given
    j1939Message.parameterGroupNumber = 0xf001U;
    j1939Message.destinationAddress = 0x32u;

    // when
    encodeCANMessage( &canMessage, &j1939Message );

    // then
    uint32_t pgn = ( uint32_t ) ( canMessage.id >> 8u );
    UNSIGNED_LONGS_EQUAL( j1939Message.parameterGroupNumber, pgn );
}

TEST( encodeMessage, given_a_j1939_message_with_protocol_format_less_than_240_when_encoding_to_CAN_message_then_CAN_id_PS_field_is_the_destination_address )
{
    // given
    j1939Message.parameterGroupNumber = 0xefffU;
    j1939Message.destinationAddress = 0x32u;
    uint32_t expectedValue = ( j1939Message.parameterGroupNumber & ~0xffu ) | j1939Message.destinationAddress;

    // when
    encodeCANMessage( &canMessage, &j1939Message );

    // then
    uint32_t pgn = ( uint32_t ) ( canMessage.id >> 8u );
    
    UNSIGNED_LONGS_EQUAL( expectedValue, pgn );
}

TEST( encodeMessage, given_a_j1939_message_when_encoding_to_CAN_message_then_message_is_extended )
{
    // given

    // when
    encodeCANMessage( &canMessage, &j1939Message );

    // then
    CHECK_TRUE( canMessage.isExtended );
}

TEST( encodeMessage, given_a_j1939_message_that_contains_data_less_or_equal_to_8_when_encoding_to_CAN_then_CAN_DLC_equal_to_j1939_data_size )
{
    // given
    uint8_t data[ 7 ] = { 1,2,3,4,5,6,7 };
	j1939Message.data = data;
	j1939Message.dataSize = sizeof( data );


    // when
    encodeCANMessage( &canMessage, &j1939Message );

    // then
    UNSIGNED_LONGS_EQUAL( j1939Message.dataSize, canMessage.dlc );
    MEMCMP_EQUAL( j1939Message.data, canMessage.data, j1939Message.dataSize );
}

TEST( encodeMessage, given_a_j1939_message_that_contains_data_bigger_than_8_when_encoding_to_CAN_then_CAN_DLC_clamped_to_8 )
{
    // given
    j1939Message.dataSize = 9u;

    // when
    encodeCANMessage( &canMessage, &j1939Message );

    // then
    UNSIGNED_LONGS_EQUAL( 8u, canMessage.dlc );
}