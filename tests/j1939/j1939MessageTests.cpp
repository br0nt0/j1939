/*******************************************************************************
 * @file	j1939MessageTests.cpp
 * @brief	
 * @author	@br0nt0
 * @date	2025
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "drivers/CANDriverSpy.h"

extern "C"
{
#include "j1939/j1939Message.h"
#include "j1939/iAddressClaimed.h"
}

TEST_GROUP( j1939Message )
{
    canDriver_t canSpy;
    j1939Message_t filledMessage;
    j1939Message_t receivedMessage;
    uint16_t parameterGroupNumber = 0xf000u;
    uint8_t data[ 8 ] = { 0x01u, 0x02u, 0x03u, 0x04u, 0x05u, 0x06u, 0x07u, 0x08u };
    uint8_t dataLength = 8u;
    uint8_t priority = 0x06u;
    uint8_t destinationAddress = 0x03u;
    uint8_t sourceAddress = 0x02u;
    uint8_t canData[ 8 ] = { 3u, 4u, 5u, 6u, 7u, 8u, 9u, 1u };
    CANMessage_t canMessage = NULL;
    void setup( void )
    {
        filledMessage = createJ1939Message( parameterGroupNumber, priority, destinationAddress, sourceAddress, data, dataLength );
        canSpy = createCANDriverSpy( );
    }
    void teardown( void )
    {
        destroyJ1939Message( filledMessage );
        destroyJ1939Message( receivedMessage );
        destroyCANDriver( canSpy );
        destroyCANMessage( canMessage );
        mock( "CANSpy" ).checkExpectations( );
		mock( ).clear( );
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
    UNSIGNED_LONGS_EQUAL( parameterGroupNumber, getJ1939MessagePGN( filledMessage ) );
    UNSIGNED_LONGS_EQUAL( priority, getJ1939MessagePriority( filledMessage ) );
    UNSIGNED_LONGS_EQUAL( destinationAddress, getJ1939MessageDA( filledMessage ) );
    UNSIGNED_LONGS_EQUAL( sourceAddress, getJ1939MessageSA( filledMessage ) );
    MEMCMP_EQUAL( data, getJ1939MessageData( filledMessage ), dataLength );
}

TEST( j1939Message, given_null_message_when_sending_j1939_message_to_CAN_layer_then_status_is_cannot_send_message )
{
    // given

    // when
    uint8_t status = sendJ1939MessageToDriver( NULL, canSpy );

    // then
    UNSIGNED_LONGS_EQUAL( CAN_TX_MESSAGE_NOT_SENT, status );
}

TEST( j1939Message, given_null_CAN_driver_when_sending_j1939_message_to_CAN_layer_then_status_is_CAN_driver_not_operational )
{
    // given

    // when
    uint8_t status = sendJ1939MessageToDriver( filledMessage, NULL );

    // then
    UNSIGNED_LONGS_EQUAL( CAN_DRIVER_NOT_OPERATIONAL, status );
}

TEST( j1939Message, given_a_non_operational_can_driver_when_sending_j1939_message_to_CAN_layer_then_status_is_can_driver_is_not_operational )
{
    // given
    mock( "CANSpy" ).expectOneCall( "isOperational" ).withPointerParameter( "base", canSpy ).andReturnValue( false );

    // when
    uint8_t status = sendJ1939MessageToDriver( filledMessage, canSpy );

    // then
    UNSIGNED_LONGS_EQUAL( CAN_DRIVER_NOT_OPERATIONAL, status );
}

TEST( j1939Message, given_a_j1939_message_with_protocol_format_equal_to_240_when_encoding_to_CAN_message_then_PS_field_is_group_extension )
{
    // given
    uint32_t id = ( getJ1939MessagePriority( filledMessage ) << 26u ) + ( getJ1939MessagePGN( filledMessage ) << 8u ) + ( getJ1939MessageSA( filledMessage ) );

    mock( "CANSpy" ).expectOneCall( "isOperational" )
        .withPointerParameter( "base", canSpy )
        .andReturnValue( true );
    mock( "CANSpy" ).expectOneCall( "sendMessage" )
        .withPointerParameter( "base", canSpy )
        .withUnsignedIntParameter( "id", id )
        .withBoolParameter( "isExtended", true )
        .withUnsignedIntParameter( "dlc", getJ1939MessageDataLength( filledMessage ) )
        .withPointerParameter( "data", getJ1939MessageData( filledMessage ) )
        .andReturnValue( CAN_TX_SUCCEEDED );

    // when
    uint8_t status = sendJ1939MessageToDriver( filledMessage, canSpy );

    // then
    UNSIGNED_LONGS_EQUAL( CAN_TX_SUCCEEDED, status );
}

TEST( j1939Message, given_a_j1939_message_with_protocol_format_less_than_240_when_encoding_to_CAN_message_then_PS_field_is_destination_address )
{
    // given
    j1939Message_t message = createJ1939Message( 0xefffu, priority, destinationAddress, sourceAddress, data, dataLength );

    uint32_t id = getJ1939MessagePriority( message ) << 26u;
    id += ( getJ1939MessagePGN( message ) >> 8u) << 16u;
    id += getJ1939MessageDA( message ) << 8u;
    id += getJ1939MessageSA( message );
    
    mock( "CANSpy" ).expectOneCall( "isOperational" )
        .withPointerParameter( "base", canSpy )
        .andReturnValue( true );
    mock( "CANSpy" ).expectOneCall( "sendMessage" )
        .withPointerParameter( "base", canSpy )
        .withUnsignedIntParameter( "id", id )
        .withBoolParameter( "isExtended", true )
        .withUnsignedIntParameter( "dlc", getJ1939MessageDataLength( message ) )
        .withPointerParameter( "data", getJ1939MessageData( message ) )
        .andReturnValue( CAN_TX_SUCCEEDED );
    
    // when
    uint8_t status = sendJ1939MessageToDriver( message, canSpy );

    // then
    UNSIGNED_LONGS_EQUAL( CAN_TX_SUCCEEDED, status );
    destroyJ1939Message( message );
}

TEST( j1939Message, given_null_driver_when_receiving_a_j1939_message_then_message_is_null )
{
    // given

    // when
    receivedMessage = receiveJ1939MessageFromDriver( NULL );

    // then
    CHECK_TRUE( NULL == receivedMessage );
}

TEST( j1939Message, given_a_CAN_driver_that_has_no_messages_returned_when_receiving_a_j1939_message_then_message_is_null )
{
    // given
    mock( "CANSpy" ).expectOneCall( "receiveMessage" )
        .withPointerParameter( "base", canSpy )
        .andReturnValue( ( void* ) 0 );

    // when
    receivedMessage = receiveJ1939MessageFromDriver( canSpy );

    // then
    CHECK_TRUE( NULL == receivedMessage );
}

TEST( j1939Message, given_a_CAN_driver_that_has_a_message_returned_when_receiving_a_message_from_it_then_message_is_converted_to_j1939_format )
{
    // given
    canMessage = createExtendedCANMessage( 0x14fb8caau, canData, 8u );
    mock( "CANSpy" ).expectOneCall( "receiveMessage" )
        .withPointerParameter( "base", canSpy )
        .andReturnValue( canMessage );

    // when
    receivedMessage = receiveJ1939MessageFromDriver( canSpy );

    // then
    UNSIGNED_LONGS_EQUAL( 0xfb8cu, getJ1939MessagePGN( receivedMessage ) );
    UNSIGNED_LONGS_EQUAL( 0xaau, getJ1939MessageSA( receivedMessage ) );
    UNSIGNED_LONGS_EQUAL( 0xffu, getJ1939MessageDA( receivedMessage ) );
    UNSIGNED_LONGS_EQUAL( 8u, getJ1939MessageDataLength( receivedMessage ) );
    MEMCMP_EQUAL( getCANMessageData( canMessage ), getJ1939MessageData( receivedMessage ), getJ1939MessageDataLength( receivedMessage ) );
}

TEST( j1939Message, given_a_CAN_message_with_PF_less_than_240_when_converting_id_to_j1939_then_destination_address_is_PS_field )
{
    // given
    canMessage = createExtendedCANMessage( 0x14efaa77u, canData, 8u );
    mock( "CANSpy" ).expectOneCall( "receiveMessage" )
        .withPointerParameter( "base", canSpy )
        .andReturnValue( canMessage );

    // when
    receivedMessage = receiveJ1939MessageFromDriver( canSpy );

    // then
    UNSIGNED_LONGS_EQUAL( 0xef00u, getJ1939MessagePGN( receivedMessage ) );
    UNSIGNED_LONGS_EQUAL( 0x77u, getJ1939MessageSA( receivedMessage ) );
    UNSIGNED_LONGS_EQUAL( 0xaau, getJ1939MessageDA( receivedMessage ) );
    UNSIGNED_LONGS_EQUAL( 8u, getJ1939MessageDataLength( receivedMessage ) );
    MEMCMP_EQUAL( getCANMessageData( canMessage ), getJ1939MessageData( receivedMessage ), getJ1939MessageDataLength( receivedMessage ) );
}

TEST( j1939Message, given_CAN_message_with_DP_1_when_decoding_to_J1939_then_pgn_is_decoded )
{
    // given
    canMessage = createExtendedCANMessage( 0x11ffff21u, canData, 8u );
    mock( "CANSpy" ).expectOneCall( "receiveMessage" )
        .withPointerParameter( "base", canSpy )
        .andReturnValue( canMessage );

    // when
    receivedMessage = receiveJ1939MessageFromDriver( canSpy );

    // then
    UNSIGNED_LONGS_EQUAL( 0x1ffffu, getJ1939MessagePGN( receivedMessage ) );
}

TEST( j1939Message, given_CAN_message_which_is_not_extended_when_decoding_to_J1939_then_message_is_null )
{
    // given
    canMessage = createStandardCANMessage( 0x7ffu, canData, 8u );
    mock( "CANSpy" ).expectOneCall( "receiveMessage" )
        .withPointerParameter( "base", canSpy )
        .andReturnValue( canMessage );

    // when
    receivedMessage = receiveJ1939MessageFromDriver( canSpy );

    // then
    CHECK_TRUE( NULL == receivedMessage );
}

TEST( j1939Message, given_CAN_message_with_EDP_1_when_converting_to_j1939_then_output_message_is_null )
{
    // given
    canMessage = createExtendedCANMessage( 0x2000000u | 0x1cf01234u, canData, 8u );
    mock( "CANSpy" ).expectOneCall( "receiveMessage" )
        .withPointerParameter( "base", canSpy )
        .andReturnValue( canMessage );

    // when
    receivedMessage = receiveJ1939MessageFromDriver( canSpy );

    // then
    CHECK_TRUE( NULL == receivedMessage );
}
