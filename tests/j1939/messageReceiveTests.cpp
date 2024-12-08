/*******************************************************************************
 * @file	messageReceiveTests.cpp
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "drivers/CANDriverSpy.h"
extern "C"
{
#include "j1939/messageReceive.h"
}

TEST_GROUP( messageReceive )
{
    canDriver_t spyCANDriver;
    void setup( void )
    {
        spyCANDriver = createCANDriverSpy( );
    }
    void teardown( void )
    {
        destroyCANDriver( spyCANDriver );
        mock( "CANSpy" ).checkExpectations( );
		mock( ).clear( );
    }
};

TEST( messageReceive, given_null_driver_when_receiving_a_j1939_message_then_message_is_null )
{
    // given

    // when
    j1939Message_t message = receiveJ1939MessageFromCANDriver( NULL );

    // then
    CHECK_TRUE( NULL == message );
}

TEST( messageReceive, given_a_spy_driver_that_has_no_messages_returned_when_receiving_a_j1939_message_then_message_is_null )
{
    // given
    mock( "CANSpy" ).expectOneCall( "receiveMessage" )
        .withPointerParameter( "base", spyCANDriver )
        .andReturnValue( ( void* ) 0 );

    // when
    j1939Message_t message = receiveJ1939MessageFromCANDriver( spyCANDriver );

    // then
    CHECK_TRUE( NULL == message );
}

TEST( messageReceive, given_a_spy_driver_when_receiving_j1939_from_it_then_CAN_message_received_and_converted_to_j1939 )
{
    // given
    uint8_t data[ 8 ] = { 3u, 4u, 5u, 6u, 7u, 8u, 9u, 1u };
    canMessageStruct_t canMessage;
    canMessage.id = 0x14fb8caau;
    canMessage.isExtended = true;
    canMessage.dlc = 8u;
    canMessage.data = data;
    mock( "CANSpy" ).expectOneCall( "receiveMessage" )
        .withPointerParameter( "base", spyCANDriver )
        .andReturnValue( &canMessage );


    // when
    j1939Message_t j1939Message = receiveJ1939MessageFromCANDriver( spyCANDriver );

    // then
    UNSIGNED_LONGS_EQUAL( 0xfb8cu, j1939Message->parameterGroupNumber );
    UNSIGNED_LONGS_EQUAL( 0xaau, j1939Message->sourceAddress );
    UNSIGNED_LONGS_EQUAL( 0xffu, j1939Message->destinationAddress );
    UNSIGNED_LONGS_EQUAL( 8u, j1939Message->dataSize );
    MEMCMP_EQUAL( canMessage.data, j1939Message->data, j1939Message->dataSize );
}


