/*******************************************************************************
 * @file	j1939StackInstanceTests.cpp
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "drivers/CANDriverSpy.h"

extern "C"
{
#include "j1939/j1939StackInstance.h"
}

TEST_GROUP( j1939StackInstance )
{
    j1939_t stack;
    canDriver_t spyCAN;
    void setup( void )
    {
        spyCAN = createCANDriverSpy( );
        stack = createJ1939StackInstance( spyCAN, 10u );
    }
    void teardown( void )
    {
        destroyJ1939Stack( stack );
        destroyCANDriver( spyCAN );
        mock( "CANSpy" ).checkExpectations( );
		mock( ).clear( );
    }
    void expectCANMessgeToSucceed( uint32_t id, bool_t isExtended, uint8_t dlc, uint8_t* data )
    {
        mock( "CANSpy" ).expectOneCall( "isOperational" )
            .withPointerParameter( "base", spyCAN )
            .andReturnValue( true );
        mock( "CANSpy" ).expectOneCall( "sendMessage" )
            .withPointerParameter( "base", spyCAN )
            .withUnsignedIntParameter( "id", id )
            .withBoolParameter( "isExtended", isExtended )
            .withUnsignedIntParameter( "dlc", dlc )
            .withPointerParameter( "data", data )
            .andReturnValue( CAN_TX_SUCCEEDED );
    }
};

TEST( j1939StackInstance, given_a_j1939_stack_instance_then_it_can_be_created_and_destroyed )
{
    // given

    // when

    // then
}

TEST( j1939StackInstance, given_a_j1939_stack_instance_with_a_NULL_dirver_then_stack_is_null )
{
    // given

    // when
    j1939_t testStack = createJ1939StackInstance( NULL, 1u );

    // then
    CHECK_TRUE( NULL == testStack );
}

TEST( j1939StackInstance, given_a_j1939_stack_instance_with_a_zero_tick_then_stack_is_null )
{
    // given

    // when
    j1939_t testStack = createJ1939StackInstance( spyCAN, 0u );

    // then
    CHECK_TRUE( NULL == testStack );
}

TEST( j1939StackInstance, given_a_j1939_stack_instance_when_sending_a_message_then_CAN_driver_picks_it_up )
{
    // given
    uint8_t data[ 8 ] = { 11u, 22u, 33u, 44u, 55u, 66u, 77u, 88u };
    j1939MessageStruct_t testMessage;
    testMessage.parameterGroupNumber = 0xfe44u;
    testMessage.priority = 7u;
    testMessage.sourceAddress = 0x0au;
    testMessage.destinationAddress = 0x0bu;
    testMessage.dataSize = 8u;
    testMessage.data = data;

    expectCANMessgeToSucceed( ( testMessage.priority << 26u ) + ( testMessage.parameterGroupNumber << 8u ) + ( testMessage.sourceAddress ),
                                true,
                                8u,
                                testMessage.data);
    // when
    uint8_t status = sendJ1939Message( stack, &testMessage );

    // then
    UNSIGNED_LONGS_EQUAL( CAN_TX_SUCCEEDED, status );
}

TEST( j1939StackInstance, given_a_j1939_stack_instance_when_receiving_a_message_from_CAN_driver_then_j1939_stack_picks_it_up )
{
    // given
    uint8_t data[ 8 ] = { 11u, 22u, 44u, 33u, 55u, 22u, 77u, 88u };
    canMessageStruct_t canMessage;
    canMessage.id = 0x14fc8cbbu;
    canMessage.isExtended = true;
    canMessage.dlc = 8u;
    canMessage.data = data;

    mock( "CANSpy" ).expectOneCall( "receiveMessage" )
        .withPointerParameter( "base", spyCAN )
        .andReturnValue( &canMessage );
    
    // when
    j1939Message_t message = receiveJ1939Message( stack  );

    // then
    UNSIGNED_LONGS_EQUAL( 0xfc8cu, message->parameterGroupNumber );
    UNSIGNED_LONGS_EQUAL( 5u, message->priority );
    UNSIGNED_LONGS_EQUAL( 0xffu, message->destinationAddress );
    UNSIGNED_LONGS_EQUAL( 0xbbu, message->sourceAddress );
    UNSIGNED_LONGS_EQUAL( 8u, message->dataSize );
    MEMCMP_EQUAL( data, message->data, message->dataSize );
}

TEST( j1939StackInstance, given_a_j1939_stack_instance_when_setting_the_source_address_then_source_address_is_returned_through_the_interface )
{
    // given
    
    // when
    setJ1939SourceAddress( stack, 0x32u );

    // then
    UNSIGNED_LONGS_EQUAL( 0x32u, getJ1939SourceAddress( stack ) );
}

TEST( j1939StackInstance, given_a_j1939_stack_instance_when_setting_the_CA_name_then_CA_name_is_returned_through_the_interface )
{
    // given
    uint8_t expectedCAName[ 8 ] = { 0x11u, 0x22u, 0x33u, 0x44u, 0x55u, 0x66u, 0x77u, 0x88u };

    // when
    setJ1939CAName( stack, expectedCAName );
    uint8_t* caName = getJ1939CAName( stack );

    // then
    MEMCMP_EQUAL( expectedCAName, caName, sizeof( expectedCAName ) );
}

TEST( j1939StackInstance, given_a_j1939_stack_instance_when_setting_the_CA_name_to_NULL_then_returned_CA_name_is_NULL )
{
    // given

    // when
    setJ1939CAName( stack, NULL );
    uint8_t* caName = getJ1939CAName( stack );

    // then
    CHECK_TRUE( NULL == caName );
}

TEST( j1939StackInstance, given_a_j1939_stack_instance_when_getting_configured_CAN_driver_then_it_is_returned_through_the_interface )
{
    // given

    // when
    canDriver_t canDriver = getJ1939ConfiguredCANDriver( stack );

    // then
    POINTERS_EQUAL( spyCAN, canDriver );
}

TEST( j1939StackInstance, given_a_j1939_stack_instance_when_getting_configured_stack_tick_in_ms_then_it_is_returned_through_the_interface )
{
    // given

    // when
    uint8_t tickMs = getJ1939ConfiguredTickMs( stack );

    // then
    UNSIGNED_LONGS_EQUAL( 10u, tickMs );
}

