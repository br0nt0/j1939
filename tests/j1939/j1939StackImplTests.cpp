/*******************************************************************************
 * @file	j1939StackImplTests.cpp
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "drivers/CANDriverSpy.h"

extern "C"
{
#include "j1939/j1939StackImpl.h"
}

TEST_GROUP( j1939StackImpl )
{
    j1939_t stack;
    canDriver_t spyCAN;
    acl_t acl;
    uint8_t name[ 8 ] = { 0xf1u, 0xf2u, 0xf3u, 0xf4u, 0xf5u, 0xf6u, 0xf7u, 0xf8u };
    j1939Message_t testMessage = NULL;
    uint8_t ticksMs = 10u;
    void setup( void )
    {
        spyCAN = createCANDriverSpy( );
        acl = createAddressClaimed( spyCAN, ticksMs, name, 0x11u );
        stack = createJ1939StackImpl( acl, spyCAN );
    }
    void teardown( void )
    {
        destroyJ1939Stack( stack );
        destroyACL( acl );
        destroyCANDriver( spyCAN );
        destroyJ1939Message( testMessage );
        mock( "CANSpy" ).checkExpectations( );
        mock( ).clear( );
    }
    void expectCANMessgeToSucceed( uint32_t id, bool_t isExtended, uint8_t dlc, uint8_t * data )
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
    void expectCANOperational( void )
    {
        mock( "CANSpy" ).expectOneCall( "isOperational" )
            .withPointerParameter( "base", spyCAN )
            .andReturnValue( true );
    }
    void expectOneCallToSendMessageWithAddressClaim( void )
    {
        expectCANOperational( );
        mock( "CANSpy" ).expectOneCall( "sendMessage" )
            .withPointerParameter( "base", spyCAN )
            .withUnsignedIntParameter( "id", 0x18eeff00u + getJ1939SourceAddress( stack ) )
            .withBoolParameter( "isExtended", true )
            .withUnsignedIntParameter( "dlc", 8u )
            .withPointerParameter( "data", getJ1939CAName( stack ) )
            .andReturnValue( CAN_TX_SUCCEEDED );
    }
    void expectNReceivedNullMessages( uint8_t n )
    {
        mock( "CANSpy" ).expectNCalls( n,  "receiveMessage" )
            .withPointerParameter( "base", spyCAN )
            .andReturnValue( ( void* ) 0 );
    }
    void expectNTimesNoBusOff( uint8_t n )
	{
		mock( "CANSpy" ).expectNCalls( n, "isTxBusOffState" )
			.withPointerParameter( "base", spyCAN )
			.andReturnValue( false );
	}
};

TEST( j1939StackImpl, given_a_j1939_stack_instance_then_it_can_be_created_and_destroyed )
{
    // given

    // when

    // then
}

TEST( j1939StackImpl, given_null_address_claim_when_creating_a_stack_then_null_returned )
{
    // given

    // when
    j1939_t testStack = createJ1939StackImpl( NULL, spyCAN );

    // then
    CHECK_TRUE( NULL == testStack );
}

TEST( j1939StackImpl, given_null_CAN_driver_when_creating_a_stack_then_null_returned )
{
    // given

    // when
    j1939_t testStack = createJ1939StackImpl( acl, NULL );

    // then
    CHECK_TRUE( NULL == testStack );
}

TEST( j1939StackImpl, given_a_j1939_message_when_sending_it_through_the_interface_then_CAN_driver_picks_it_up )
{
    // given
    uint8_t data[ 8 ] = { 11u, 22u, 33u, 44u, 55u, 66u, 77u, 88u };
    testMessage = createJ1939Message( 0xfe44u, 7u, 0x0bu, 0x0au, data, 8u );

    expectCANMessgeToSucceed( ( getJ1939MessagePriority( testMessage ) << 26u ) + ( getJ1939MessagePGN( testMessage ) << 8u ) + getJ1939MessageSA( testMessage ),
                                true,
                                8u,
                                getJ1939MessageData( testMessage ) );
    // when
    uint8_t status = sendJ1939Message( stack, testMessage );

    // then
    UNSIGNED_LONGS_EQUAL( CAN_TX_SUCCEEDED, status );
}

TEST( j1939StackImpl, given_a_j1939_stack_instance_when_setting_the_source_address_then_source_address_is_returned_through_the_interface )
{
    // given
    
    // when
    setJ1939SourceAddress( stack, 0x32u );

    // then
    UNSIGNED_LONGS_EQUAL( 0x32u, getJ1939SourceAddress( stack ) );
}

TEST( j1939StackImpl, given_a_CA_name_when_setting_the_value_then_it_is_returned )
{
    // given
    uint8_t expectedCAName[ 8 ] = { 0x11u, 0x22u, 0x33u, 0x44u, 0x55u, 0x66u, 0x77u, 0x88u };

    // when
    setJ1939CAName( stack, expectedCAName );
    uint8_t* caName = getJ1939CAName( stack );

    // then
    MEMCMP_EQUAL( expectedCAName, caName, sizeof( expectedCAName ) );
}

TEST( j1939StackImpl, given_a_j1939_stack_instance_when_setting_the_CA_name_to_NULL_then_returned_CA_name_is_NULL )
{
    // given

    // when
    setJ1939CAName( stack, NULL );
    uint8_t* caName = getJ1939CAName( stack );

    // then
    CHECK_TRUE( NULL == caName );
}

TEST( j1939StackImpl, given_no_received_messages_when_updating_core_scheduler_then_address_claim_message_sent )
{
    // given
    expectNReceivedNullMessages( 1u );
    expectOneCallToSendMessageWithAddressClaim( );

    // when
    updateJ1939CoreScheduler( stack );

    // then
}

TEST( j1939StackImpl, given_2_messages_in_mailbox_but_no_acl_contention_for_more_than_250_ms_when_updating_core_scheduler_then_address_was_claimed )
{
    // given
    uint8_t data[ 8 ] = { 3u, 4u, 5u, 6u, 7u, 8u, 9u, 1u };
    CANMessage_t canMessage = createExtendedCANMessage( 0x14fb8caau, data, 8u );
    mock( "CANSpy" ).expectNCalls( 2, "receiveMessage" )
        .withPointerParameter( "base", spyCAN )
        .andReturnValue( canMessage );

    uint8_t howManyUpdatesIn250Ms = ( uint8_t ) ( 250u / ticksMs );

    expectNReceivedNullMessages( howManyUpdatesIn250Ms + 1u );
    expectNTimesNoBusOff( howManyUpdatesIn250Ms );
    expectOneCallToSendMessageWithAddressClaim( );

    // when
    for ( uint8_t i = 0u; i < ( howManyUpdatesIn250Ms + 1u ); i++ )
    {
        updateJ1939CoreScheduler( stack );
    }

    // then
    CHECK_TRUE( wasJ1939AddressClaimed( stack ) );
    destroyCANMessage( canMessage );
}

TEST( j1939StackImpl, given_normal_traffic_mode_and_a_received_address_claim_request_when_updating_core_scheduler_then_ACL_is_registered_and_sent )
{
    // given
    uint8_t data[ 3 ];
    data[ 0 ] = J1939_AC & 0xffu;
    data[ 1 ] = ( J1939_AC >> 8u ) & 0xffu;
    data[ 2 ] = ( J1939_AC >> 16u ) & 0xffu;
    CANMessage_t canMessage = createExtendedCANMessage( 0x18eaa300u, data, 3u );
    mock( "CANSpy" ).expectOneCall( "receiveMessage" ).withPointerParameter( "base", spyCAN ).andReturnValue( canMessage );
    mock( "CANSpy" ).expectOneCall( "receiveMessage" ).withPointerParameter( "base", spyCAN ).andReturnValue( (void*) 0 );

    setACLStateMachineState( acl, NORMAL_TRAFFIC );
    expectOneCallToSendMessageWithAddressClaim( );

    // when
    updateJ1939CoreScheduler( stack );

    // then
    destroyCANMessage( canMessage );
}



