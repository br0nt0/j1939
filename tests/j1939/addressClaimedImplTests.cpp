/*******************************************************************************
 * @file	addressClaimedImplTests.cpp
 * @brief	
 * @author	@br0nt0
 * @date	2025
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "drivers/CANDriverSpy.h"

extern "C"
{
#include "j1939/addressClaimedImpl.h"
}

TEST_GROUP( addressClaimedImpl )
{
    canDriver_t spyCANDriver;
    acl_t acl;
    uint8_t caName[ 8 ] = { 0xf1u, 0xf2u, 0xf3u, 0xf4u, 0xf5u, 0xf6u, 0xf7u, 0xf8u };
    uint16_t ticksMs = 10u;
    void setup( void )
    {
        spyCANDriver = createCANDriverSpy( );
        acl = createAddressClaimed( spyCANDriver, ticksMs, caName, 0x11u );
    }
    void teardown( void )
    {
        destroyACL( acl );
        destroyCANDriver( spyCANDriver );
        mock( "CANSpy" ).checkExpectations( );
        mock( ).clear( );
    }
    void expectCANOperational( void )
	{
		mock( "CANSpy" ).expectOneCall( "isOperational" )
			.withPointerParameter( "base", spyCANDriver )
			.andReturnValue( true );
    }
    void expectNoCallToSendMessage( void )
	{
		mock( "CANSpy" ).expectNoCall( "isOperational" );
		mock( "CANSpy" ).expectNoCall( "sendMessage" );
	}
    void expectOneCallToSendMessageWithAddressClaim( void )
	{
        expectCANOperational( );
		mock( "CANSpy" ).expectOneCall( "sendMessage" )
			.withPointerParameter( "base", spyCANDriver )
            .withUnsignedIntParameter( "id", 0x18eeff00u + getACLSourceAddress( acl ) )
            .withBoolParameter( "isExtended", true )
            .withUnsignedIntParameter( "dlc", 8u )
            .withPointerParameter( "data", getCAName( acl ) )
			.andReturnValue( CAN_TX_SUCCEEDED );
    }
    void expectOneCallToSendMessageWithTxBufferFull( void )
	{
		expectCANOperational( );
		mock( "CANSpy" ).expectOneCall( "sendMessage" )
			.withPointerParameter( "base", spyCANDriver )
			.withUnsignedIntParameter( "id", 0x18eeff00u + getACLSourceAddress( acl ) )
			.withBoolParameter( "isExtended", true )
			.withUnsignedIntParameter( "dlc", 8u )
			.withPointerParameter( "data", getCAName( acl ) )
			.andReturnValue( CAN_TX_BUFFER_FULL );
    }
    void expectAddressClaimWithAddress( uint8_t address )
	{
		expectCANOperational( );
        mock( "CANSpy" ).expectOneCall( "sendMessage" )
            .withPointerParameter( "base", spyCANDriver )
            .withUnsignedIntParameter( "id", 0x18eeff00u + address )
            .withBoolParameter( "isExtended", true )
            .withUnsignedIntParameter( "dlc", 8u )
            .withPointerParameter( "data", getCAName( acl ) )
			.andReturnValue( CAN_TX_SUCCEEDED );
    }
    void expectNoAddressAvailableForClaim( uint8_t* contenderName )
	{
		for ( uint8_t i = 0u; i < 120u; i++ )
		{
			expectNoBusOff( 1u );
			registerACLContention( acl, contenderName );
			expectAddressClaimWithAddress(  128u + i );
			updateACLStateMachine( acl );
		}
    }
    void expectCannotClaimMessge( void )
	{
		expectCANOperational( );
		mock( "CANSpy" ).expectOneCall( "sendMessage" )
			.withPointerParameter( "base", spyCANDriver )
			.withUnsignedIntParameter( "id", 0x18eefffeu )
			.andReturnValue( CAN_TX_SUCCEEDED )
			.ignoreOtherParameters( );
	}
    void expectOneBusOffEvent( void )
    {
        mock( "CANSpy" ).expectOneCall( "isTxBusOffState" ).withPointerParameter( "base", spyCANDriver ).andReturnValue( true );
    }
    void expectNoBusOff( uint8_t n )
	{
		mock( "CANSpy" ).expectNCalls( n, "isTxBusOffState" )
			.withPointerParameter( "base", spyCANDriver )
			.andReturnValue( false );
	}
    uint16_t createPseudoDelay( uint8_t* name, uint16_t ticks )
	{
		uint16_t pseudoDelay = 0u;
		for ( uint8_t i = 0u; i < 8u; i++ )
		{
			pseudoDelay += name[ i ];
		}
		pseudoDelay %= 255;
		pseudoDelay *= 6u;
		pseudoDelay = ( uint16_t ) ( pseudoDelay / 10u );
		pseudoDelay /= ticks;

		return ( pseudoDelay );
	}
};

TEST( addressClaimedImpl, given_an_acl_implementation_then_it_can_be_created_and_destroyed )
{
    // given

    // when

    // then
}

TEST( addressClaimedImpl, given_a_null_CAN_driver_when_creating_an_addres_claim_implementation_then_null_is_returned )
{
    // given

    // when

    // then
    CHECK_TRUE( NULL == createAddressClaimed( NULL, 10u, caName, 0x00u ) );
}

TEST( addressClaimedImpl, given_zero_ms_tick_when_creating_an_addres_claim_implementation_then_null_is_returned )
{
    // given

    // when

    // then
    CHECK_TRUE( NULL == createAddressClaimed( spyCANDriver, 0u, caName, 0x00u ) );
}

TEST( addressClaimedImpl, given_null_CA_name_when_creating_an_addres_claim_implementation_then_null_is_returned )
{
    // given

    // when

    // then
    CHECK_TRUE( NULL == createAddressClaimed( spyCANDriver, 10u, NULL, 0x00u ) );
}

TEST( addressClaimedImpl, given_a_type_when_creating_an_addres_claim_implementation_then_it_is_returned )
{
    // given

    // when

    // then
    STRCMP_EQUAL( "ACLv1", getACLType( acl ) );
}

TEST( addressClaimedImpl, given_the_init_state_when_updating_acl_state_machine_then_acl_message_sent )
{
    // given
    expectOneCallToSendMessageWithAddressClaim( );

    // when
    updateACLStateMachine( acl );

    // then
    STRCMP_EQUAL( "ACLv1", getACLType( acl ) );
}

TEST( addressClaimedImpl, given_a_failed_message_transmitt_when_in_init_state_then_acl_message_sent_with_next_iteration )
{
    // given
	expectOneCallToSendMessageWithTxBufferFull( );
	expectOneCallToSendMessageWithAddressClaim( );

    // when
    updateACLStateMachine( acl );
    updateACLStateMachine( acl );

    // then
}

TEST( addressClaimedImpl, given_no_external_factors_when_updating_address_claim_routine_then_address_successfully_claimed_after_250ms_from_sent_message )
{
    // given
    expectOneCallToSendMessageWithAddressClaim( );
    mock( "CANSpy" ).ignoreOtherCalls( );

    // when
    updateACLStateMachine( acl );
    for ( size_t i = 0; i < ( CONTENTION_TIMEOUT_MS / ticksMs ); i++ )
	{
		updateACLStateMachine( acl );
	}

    // then
    UNSIGNED_LONGS_EQUAL( NORMAL_TRAFFIC, getACLStateMachineState( acl ) );
    CHECK_TRUE( wasACLSuccessful( acl ) );
}

TEST( addressClaimedImpl, given_bus_off_event_when_in_wait_for_contention_state_then_a_random_delay_is_created_from_name_until_address_reclaim )
{
    // given
    setACLStateMachineState( acl, WAIT_FOR_CONTENTION );
    expectOneCallToSendMessageWithAddressClaim( );
    expectOneBusOffEvent( );

    uint16_t pseudoDelay = createPseudoDelay( caName, ticksMs );

    // when
    updateACLStateMachine( acl );
    UNSIGNED_LONGS_EQUAL( DELAY_BEFORE_RECLAIM, getACLStateMachineState( acl ) );

    for ( uint16_t i = 0u; i < ( pseudoDelay + 1u ); i++ )
	{
		updateACLStateMachine( acl );
    }

    // then
    UNSIGNED_LONGS_EQUAL( WAIT_FOR_CONTENTION, getACLStateMachineState( acl ) );
}

TEST( addressClaimedImpl, given_bus_off_event_with_transmit_buffer_full_when_in_wait_for_contention_state_then_address_claim_message_is_sent_after_a_random_delay_and_tx_buffer_not_full )
{
    // given
    setACLStateMachineState( acl, WAIT_FOR_CONTENTION );
    expectOneCallToSendMessageWithTxBufferFull( );
    expectOneCallToSendMessageWithAddressClaim( );
    expectOneBusOffEvent( );

    uint16_t pseudoDelay = createPseudoDelay( caName, ticksMs );

    // when
    updateACLStateMachine( acl );
    UNSIGNED_LONGS_EQUAL( DELAY_BEFORE_RECLAIM, getACLStateMachineState( acl ) );

    for ( uint16_t i = 0u; i < ( pseudoDelay + 1u ); i++ )
	{
		updateACLStateMachine( acl );
    }
    updateACLStateMachine( acl );

    // then
    UNSIGNED_LONGS_EQUAL( WAIT_FOR_CONTENTION, getACLStateMachineState( acl ) );
}

TEST( addressClaimedImpl, given_a_contending_message_with_name_bigger_than_mine_when_in_wait_for_contention_state_then_address_reclaimed )
{
    // given
    uint8_t contenderName[ 8 ] = { 0xf1u, 0xf2u, 0xf3u, 0xf4u, 0xf5u, 0xf6u, 0xf7u, 0xf8u + 1u };

    setACLStateMachineState( acl, WAIT_FOR_CONTENTION );
    expectOneCallToSendMessageWithTxBufferFull( );
    expectOneCallToSendMessageWithAddressClaim( );
    mock( "CANSpy" ).ignoreOtherCalls( );

    registerACLContention( acl, contenderName );

    // when
    updateACLStateMachine( acl );
    updateACLStateMachine( acl );

    // then
    UNSIGNED_LONGS_EQUAL( WAIT_FOR_CONTENTION, getACLStateMachineState( acl ) );
}

TEST( addressClaimedImpl, given_a_contending_message_with_name_equal_to_mine_when_in_wait_for_contention_state_then_address_changed_to_128 )
{
    // given
    uint8_t contenderName[ 8 ] = { 0xf1u, 0xf2u, 0xf3u, 0xf4u, 0xf5u, 0xf6u, 0xf7u, 0xf8u };

    setACLStateMachineState( acl, WAIT_FOR_CONTENTION );

    expectNoBusOff( 1 );
    registerACLContention( acl, contenderName );
    expectAddressClaimWithAddress( 128u );

    // when
    updateACLStateMachine( acl );

    // then
    UNSIGNED_LONGS_EQUAL( 128, getACLSourceAddress( acl ) );
}

TEST( addressClaimedImpl, given_contention_and_no_available_address_when_prioritizing_contention_then_cannot_claim_message_sent )
{
    // given
    uint8_t contenderName[ 8 ] = { 0xf1u, 0xf2u, 0xf3u, 0xf4u, 0xf5u, 0xf6u, 0xf7u, 0xf8u };

    setACLStateMachineState( acl, WAIT_FOR_CONTENTION );
    expectNoAddressAvailableForClaim( contenderName );
    expectNoBusOff( 1 );
    registerACLContention( acl, contenderName );
    expectCannotClaimMessge( );

    // when
    updateACLStateMachine( acl );
    updateACLStateMachine( acl );
    updateACLStateMachine( acl );

    // then
    UNSIGNED_LONGS_EQUAL( CANNOT_CLAIM_ADDRESS, getACLStateMachineState( acl ) );
    UNSIGNED_LONGS_EQUAL( 254u, getACLSourceAddress( acl ) );
}

TEST( addressClaimedImpl, given_a_request_for_address_claim_when_cannot_claim_address_then_a_pseudo_delay_is_applied_before_sending_a_cannot_claim_messge )
{
    // given
    setACLStateMachineState( acl, CANNOT_CLAIM_ADDRESS );
    registerRequestForACL( acl );
    setACLSourceAddress( acl, 254u );

    uint16_t pseudoDelay = createPseudoDelay( caName, ticksMs );
    expectCannotClaimMessge( );

    // when
    updateACLStateMachine( acl );
    UNSIGNED_LONGS_EQUAL( DELAY_BEFORE_CANNOT_CLAIM, getACLStateMachineState( acl ) );
    for ( uint16_t i = 0u; i < ( pseudoDelay + 1u ); i++ )
    {
        updateACLStateMachine( acl );
    }
    updateACLStateMachine( acl );

    // then
    UNSIGNED_LONGS_EQUAL( CANNOT_CLAIM_ADDRESS, getACLStateMachineState( acl ) );
    UNSIGNED_LONGS_EQUAL( 254u, getACLSourceAddress( acl ) );
}

TEST( addressClaimedImpl, given_a_request_for_address_claim_when_in_normal_traffic_state_then_address_claim_message_is_sent )
{
    // given
    setACLStateMachineState( acl, NORMAL_TRAFFIC );
    registerRequestForACL( acl );
    expectOneCallToSendMessageWithAddressClaim( );

    // when
    updateACLStateMachine( acl );
    
    // then
    UNSIGNED_LONGS_EQUAL( NORMAL_TRAFFIC, getACLStateMachineState( acl ) );
}

TEST( addressClaimedImpl, given_a_request_for_address_claim_when_in_normal_traffic_state_and_tx_buffer_full_then_address_claim_message_is_sent_when_buffer_has_slot )
{
    // given
    setACLStateMachineState( acl, NORMAL_TRAFFIC );
    registerRequestForACL( acl );
    expectOneCallToSendMessageWithTxBufferFull( );
    expectOneCallToSendMessageWithAddressClaim( );

    // when
    updateACLStateMachine( acl );
    updateACLStateMachine( acl );

    // then
    UNSIGNED_LONGS_EQUAL( NORMAL_TRAFFIC, getACLStateMachineState( acl ) );
}

TEST( addressClaimedImpl, given_a_received_message_with_own_source_address_when_in_normal_traffic_state_then_address_claim_message_is_sent )
{
    // given
    setACLStateMachineState( acl, NORMAL_TRAFFIC );
    registerRcvMessageWithOwnSourceAddress( acl );
    expectOneCallToSendMessageWithAddressClaim( );

    // when
    updateACLStateMachine( acl );

    // then
    UNSIGNED_LONGS_EQUAL( NORMAL_TRAFFIC, getACLStateMachineState( acl ) );
}

TEST( addressClaimedImpl, given_no_contention_or_claim_or_message_with_own_SA_when_in_normal_traffic_state_then_nothing_happens )
{
    // given
    setACLStateMachineState( acl, NORMAL_TRAFFIC );
    expectNoCallToSendMessage( );

    // when
    updateACLStateMachine( acl );
    updateACLStateMachine( acl );
    updateACLStateMachine( acl );
    updateACLStateMachine( acl );
    updateACLStateMachine( acl );
    updateACLStateMachine( acl );
    updateACLStateMachine( acl );

    // then
    UNSIGNED_LONGS_EQUAL( NORMAL_TRAFFIC, getACLStateMachineState( acl ) );
}


TEST( addressClaimedImpl, given_a_contention_with_name_greater_than_mine_when_in_normal_traffic_state_then_address_is_reclaimed )
{
    // given
    uint8_t contenderName[ 8 ] = { 0xf1u, 0xf2u, 0xf3u, 0xf4u, 0xf5u, 0xf6u, 0xf7u, 0xf8u + 1u };
    setACLStateMachineState( acl, NORMAL_TRAFFIC );
    registerACLContention( acl, contenderName );
    expectOneCallToSendMessageWithAddressClaim( );

    // when
    updateACLStateMachine( acl );

    // then
    UNSIGNED_LONGS_EQUAL( NORMAL_TRAFFIC, getACLStateMachineState( acl ) );
}

TEST( addressClaimedImpl, given_a_contention_with_name_equal_to_mine_when_in_normal_traffic_state_then_new_address_selected_and_claimed )
{
    // given
    uint8_t contenderName[ 8 ] = { 0xf1u, 0xf2u, 0xf3u, 0xf4u, 0xf5u, 0xf6u, 0xf7u, 0xf8u };
    setACLStateMachineState( acl, NORMAL_TRAFFIC );
    registerACLContention( acl, contenderName );
    expectAddressClaimWithAddress( 128u );

    // when
    updateACLStateMachine( acl );

    // then
    UNSIGNED_LONGS_EQUAL( NORMAL_TRAFFIC, getACLStateMachineState( acl ) );
    UNSIGNED_LONGS_EQUAL( 128u, getACLSourceAddress( acl ) );
}

TEST( addressClaimedImpl, given_a_CA_name_then_it_can_be_set_and_get )
{
    // given
    uint8_t name[ 8 ] = { 1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u };

    // when
    setCAName( acl, name );

    // then
    MEMCMP_EQUAL( name, getCAName( acl ), 8u );
}

