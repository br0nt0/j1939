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


TEST_GROUP( decodeMessage )
{
    canMessageStruct_t canMessage;
    j1939MessageStruct_t j1939Message;
    void setup( void )
    {
        canMessage.isExtended = true;
    }
    void teardown( void )
    {
    }
    void checkMessageIsZero( void )
    {
        UNSIGNED_LONGS_EQUAL( 0u, j1939Message.parameterGroupNumber );
        UNSIGNED_LONGS_EQUAL( 0u, j1939Message.priority );
        UNSIGNED_LONGS_EQUAL( 0u, j1939Message.sourceAddress );
        UNSIGNED_LONGS_EQUAL( 0u, j1939Message.destinationAddress );
        UNSIGNED_LONGS_EQUAL( 0u, j1939Message.dataSize );
        CHECK_TRUE( NULL == j1939Message.data );
    }
};

TEST( decodeMessage, given_null_inputs_to_the_converter_then_nothing_happens )
{
    // given

    // when
    decodeMessage( NULL, NULL );
    decodeMessage( &j1939Message, NULL );
    decodeMessage( NULL, &canMessage );

    // then
}

TEST( decodeMessage, given_a_non_extended_CAN_message_when_decoding_to_J1939_then_j1939_message_defaults_to_zero )
{
    // given
    canMessage.isExtended = false;

    // when
    decodeMessage( &j1939Message, &canMessage );

    // then
    checkMessageIsZero( );
}

TEST( decodeMessage, given_CAN_message_with_the_protocol_format_less_than_240_when_decoding_to_J1939_then_j1939_message_destination_address_is_PS_field )
{
    // given
    canMessage.id = 0x14efaa77u;

    // when
    decodeMessage( &j1939Message, &canMessage );

    // then
    UNSIGNED_LONGS_EQUAL( 0xaau, j1939Message.destinationAddress );
}

TEST( decodeMessage, given_CAN_message_with_the_protocol_format_equal_to_240_when_decoding_to_J1939_then_j1939_message_destination_address_is_global_address )
{
    // given
    canMessage.id = 0x14f0aa77u;

    // when
    decodeMessage( &j1939Message, &canMessage );

    // then
    UNSIGNED_LONGS_EQUAL( CAN_GLOBAL_ADDRESS, j1939Message.destinationAddress );
}

TEST( decodeMessage, given_CAN_message_with_the_protocol_format_greater_than_240_when_decoding_to_J1939_then_j1939_message_destination_address_is_global_address )
{
    // given
    canMessage.id = 0x14f1aa77u;

    // when
    decodeMessage( &j1939Message, &canMessage );

    // then
    UNSIGNED_LONGS_EQUAL( CAN_GLOBAL_ADDRESS, j1939Message.destinationAddress );
}

TEST( decodeMessage, given_CAN_message_when_decoding_to_J1939_then_pgn_is_decoded )
{
    // given
    canMessage.id = 0x14f0aa12u;

    // when
    decodeMessage( &j1939Message, &canMessage );

    // then
    UNSIGNED_LONGS_EQUAL( 0xf0aa, j1939Message.parameterGroupNumber );
}

TEST( decodeMessage, given_CAN_message_with_DP_1_when_decoding_to_J1939_then_pgn_is_decoded )
{
    // given
    canMessage.id = 0x11ffff21u;

    // when
    decodeMessage( &j1939Message, &canMessage );

    // then
    UNSIGNED_LONGS_EQUAL( 0x1ffffu, j1939Message.parameterGroupNumber );
}

TEST( decodeMessage, given_CAN_message_with_DP_0_and_PF_less_than_240_when_decoding_to_J1939_then_pgn_is_decoded )
{
    // given
    canMessage.id = 0x18ef2134u;

    // when
    decodeMessage( &j1939Message, &canMessage );

    // then
    UNSIGNED_LONGS_EQUAL( 0xef00, j1939Message.parameterGroupNumber );
}

TEST( decodeMessage, given_CAN_message_with_DP_1_and_PF_less_than_240_when_decoding_to_J1939_then_pgn_is_decoded )
{
    // given
    canMessage.id = 0x19ef2134;

    // when
    decodeMessage( &j1939Message, &canMessage );

    // then
    UNSIGNED_LONGS_EQUAL( 0x1ef00, j1939Message.parameterGroupNumber );
}

TEST( decodeMessage, given_CAN_message_with_EDP_1_when_decoding_to_J1939_then_j1939_message_set_to_zero )
{
    // given
    canMessage.id = ( 1u << 25u ) | 0x1cf01234u;
    

    // when
    decodeMessage( &j1939Message, &canMessage );

    // then
    checkMessageIsZero( );
}

TEST( decodeMessage, given_CAN_message_when_decoding_to_J1939_then_source_address_is_decoded )
{
    // given
    canMessage.id = 0x14efaa12u;

    // when
    decodeMessage( &j1939Message, &canMessage );

    // then
    UNSIGNED_LONGS_EQUAL( 0x12u, j1939Message.sourceAddress );
}

TEST( decodeMessage, given_CAN_message_when_decoding_to_J1939_then_priority_is_decoded )
{
    // given
    canMessage.id = 0x14efaa12u;

    // when
    decodeMessage( &j1939Message, &canMessage );

    // then
    UNSIGNED_LONGS_EQUAL( 5u, j1939Message.priority );
}

TEST( decodeMessage, given_CAN_message_when_decoding_to_J1939_then_data_and_size_decoded_accordingly )
{
    // given
    uint8_t data[ 7 ] = { 1U, 2U, 3U, 4U, 5U, 6U, 7U };
	canMessage.data = data;
	canMessage.dlc = sizeof( data );

    // when
    decodeMessage( &j1939Message, &canMessage );

    // then
    UNSIGNED_LONGS_EQUAL( sizeof( data ), j1939Message.dataSize );
	MEMCMP_EQUAL( &data, j1939Message.data, j1939Message.dataSize );
}


