/*******************************************************************************
 * @file	addressClaimedTests.cpp
 * @brief
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "drivers/CANDriverSpy.h"

extern "C"
{
#include "j1939/addressClaimed.h"
#include "j1939/j1939StackInstance.h"
}

TEST_GROUP( addressClaimed )
{
	canDriver_t spyCanDriver;
	aclStruct_t acl;
	uint8_t caName[ 8 ] = { 0xf1u, 0xf2u, 0xf3u, 0xf4u, 0xf5u, 0xf6u, 0xf7u, 0xf8u };
	void setup( void )
	{
		spyCanDriver = createCANDriverSpy( );
		acl.caName = caName;
		acl.tickMs = 10u;
		acl.driver = spyCanDriver;
		setAddressClaimInitialState( &acl, UNDEFINED );
		acl.sourceAddress = 0x01u;
	}
	void teardown( void )
	{
		destroyCANDriver( spyCanDriver );
		mock( "CANSpy" ).checkExpectations( );
		mock( ).clear( );
	}
	void expectCANOperational( void )
	{
		mock( "CANSpy" ).expectOneCall( "isOperational" )
			.withPointerParameter( "base", spyCanDriver )
			.andReturnValue( true );
	}
	void expectNoCallToSendMessage( void )
	{
		mock( "CANSpy" ).expectNoCall( "isOperational" );
		mock( "CANSpy" ).expectNoCall( "sendMessage" );
	}
	void expectOneCallToSendMessageWithAddressClaim( void )
	{
		canMessageStruct_t expectedCANMessage;
		expectedCANMessage.id = 0x18eeff00u + acl.sourceAddress;
		expectedCANMessage.isExtended = true;
		expectedCANMessage.dlc = 8u;
		expectedCANMessage.data = acl.caName;
		expectCANOperational( );
		mock( "CANSpy" ).expectOneCall( "sendMessage" )
			.withPointerParameter( "base", spyCanDriver )
			.withUnsignedIntParameter( "id", expectedCANMessage.id )
			.withBoolParameter( "isExtended", expectedCANMessage.isExtended )
			.withUnsignedIntParameter( "dlc", expectedCANMessage.dlc )
			.withPointerParameter( "data", expectedCANMessage.data )
			.andReturnValue( CAN_TX_SUCCEEDED );
	}
	void expectOneCallToSendMessageWithTxBufferFull( void )
	{
		canMessageStruct_t expectedCANMessage;
		expectedCANMessage.id = 0x18eeff00u + acl.sourceAddress;
		expectedCANMessage.isExtended = true;
		expectedCANMessage.dlc = 8u;
		expectedCANMessage.data = acl.caName;
		expectCANOperational( );
		mock( "CANSpy" ).expectOneCall( "sendMessage" )
			.withPointerParameter( "base", spyCanDriver )
			.withUnsignedIntParameter( "id", expectedCANMessage.id )
			.withBoolParameter( "isExtended", expectedCANMessage.isExtended )
			.withUnsignedIntParameter( "dlc", expectedCANMessage.dlc )
			.withPointerParameter( "data", expectedCANMessage.data )
			.andReturnValue( CAN_TX_BUFFER_FULL );
	}
	void expectCannotClaimMessge( void )
	{
		expectCANOperational( );
		mock( "CANSpy" ).expectOneCall( "sendMessage" )
			.withPointerParameter( "base", spyCanDriver )
			.withUnsignedIntParameter( "id", 0x18eefffeu )
			.andReturnValue( CAN_TX_SUCCEEDED )
			.ignoreOtherParameters( );
	}
	void expectAddressClaimWithAddress( uint8_t address )
	{
		expectCANOperational( );
		mock( "CANSpy" ).expectOneCall( "sendMessage" )
			.withPointerParameter( "base", spyCanDriver )
			.withUnsignedIntParameter( "id", 0x18eeff00u + address )
			.withBoolParameter( "isExtended", true )
			.withUnsignedIntParameter( "dlc", 8u )
			.withPointerParameter( "data", acl.caName )
			.andReturnValue( CAN_TX_SUCCEEDED );
	}	
	void expectNoAddressAvailableForClaim( uint8_t* contenderName )
	{
		for ( uint8_t i = 0u; i < 120u; i++ )
		{
			expectNoBusOff( 1u );
			registerReceivedContention( &acl, contenderName );
			expectAddressClaimWithAddress(  128u + i );
			updateAddressClaimed( &acl );
		}
	}
	void expectNoBusOff( uint8_t n )
	{
		mock( "CANSpy" ).expectNCalls( n, "isTxBusOffState" )
			.withPointerParameter( "base", spyCanDriver )
			.andReturnValue( false );
	}
	uint16_t createPseudoDelay( uint8_t * name, uint8_t ticksMs )
	{
		uint16_t pseudoDelay = 0u;
		for ( uint8_t i = 0u; i < 8u; i++ )
		{
			pseudoDelay += name[ i ];
		}
		pseudoDelay %= 255;
		pseudoDelay *= 6u;
		pseudoDelay = ( uint16_t ) ( pseudoDelay / 10u );
		pseudoDelay /= ticksMs;

		return ( pseudoDelay );
	}
};

TEST( addressClaimed, given_null_item__when_configuring_address_claimed_procedure_then_state_is_undefined )
{
	// given
	setAddressClaimInitialState( NULL, INIT );

	// when

	// then
	UNSIGNED_LONGS_EQUAL( UNDEFINED, acl.state );
}

TEST( addressClaimed, given_null_CAN_driver_when_configuring_address_claimed_procedure_then_state_is_undefined )
{
	// given
	acl.driver = NULL;
	setAddressClaimInitialState( &acl, INIT );

	// when

	// then
	UNSIGNED_LONGS_EQUAL( UNDEFINED, acl.state );
}

TEST( addressClaimed, given_null_CA_name_when_configuring_address_claimed_procedure_then_state_is_undefined )
{
	// given
	acl.caName = NULL;
	setAddressClaimInitialState( &acl, INIT );

	// when

	// then
	UNSIGNED_LONGS_EQUAL( UNDEFINED, acl.state );
}

TEST( addressClaimed, given_zero_ticks_when_configuring_address_claimed_procedure_then_state_is_undefined )
{
	// given
	acl.tickMs = 0u;
	setAddressClaimInitialState( &acl, INIT );

	// when

	// then
	UNSIGNED_LONGS_EQUAL( UNDEFINED, acl.state );
}

TEST( addressClaimed, given_undefined_state_when_updating_address_claimed_procedure_then_nothing_happens )
{
	// given

	// when
	updateAddressClaimed( &acl );
	updateAddressClaimed( &acl );
	updateAddressClaimed( &acl );
	updateAddressClaimed( &acl );

	// then
	UNSIGNED_LONGS_EQUAL( UNDEFINED, acl.state );
}

TEST( addressClaimed, given_null__when_configuring_address_claimed_procedure_then_nothing_happens )
{
	// given
	acl.driver = NULL;
	setAddressClaimInitialState( NULL, UNDEFINED );

	// when
	updateAddressClaimed( NULL );

	// then
}

TEST( addressClaimed, given_an_address_and_valid_config_when_in_init_state_then_an_address_claimed_messaged_is_sent_to_CAN_driver )
{
	// given
	acl.state = INIT;
	expectOneCallToSendMessageWithAddressClaim( );

	// when
	updateAddressClaimed( &acl );

	// then
}

TEST( addressClaimed, given_a_failed_message_transmit_when_in_init_state_then_an_address_claimed_messaged_is_sent_to_CAN_driver_with_next_iteration )
{
	// given
	acl.state = INIT;

	expectOneCallToSendMessageWithTxBufferFull( );
	expectOneCallToSendMessageWithAddressClaim( );

	// when
	updateAddressClaimed( &acl );
	updateAddressClaimed( &acl );

	// then
}

TEST( addressClaimed, given_no_address_content_and_no_bus_off_when_in_wait_for_contention_state_then_address_successfully_claimed_after_250ms )
{
	// given
	acl.state = WAIT_FOR_CONTENTION;
	expectNoBusOff( CONTENTION_TIMEOUT_MS / acl.tickMs );

	// when
	for ( size_t i = 0; i < ( CONTENTION_TIMEOUT_MS / acl.tickMs ); i++ )
	{
		updateAddressClaimed( &acl );
	}

	// then
	CHECK_TRUE( wasAddressClaimedSuccessfuly( &acl ) );
}

TEST( addressClaimed, given_bus_off_event_when_in_wait_for_contention_state_then_a_random_delay_is_created_from_name_until_address_reclaim )
{
	// given
	acl.state = WAIT_FOR_CONTENTION;

	mock( "CANSpy" ).expectOneCall( "isTxBusOffState" ).withPointerParameter( "base", spyCanDriver ).andReturnValue( true );
	expectOneCallToSendMessageWithAddressClaim( );
	uint16_t pseudoDelay = createPseudoDelay( acl.caName, acl.tickMs );

	// when
	updateAddressClaimed( &acl ); // transition to a delay handling state
	for ( uint16_t i = 0u; i < ( pseudoDelay + 1u ); i++ )
	{
		updateAddressClaimed( &acl ); // processing
	}

	// then
}

TEST( addressClaimed, given_bus_off_event_and_tx_buffer_full_when_in_wait_for_contention_state_then_a_random_delay_is_created_from_name_until_address_reclaim )
{
	// given
	acl.state = WAIT_FOR_CONTENTION;

	mock( "CANSpy" ).expectOneCall( "isTxBusOffState" ).withPointerParameter( "base", spyCanDriver ).andReturnValue( true );
	expectOneCallToSendMessageWithTxBufferFull( );
	expectOneCallToSendMessageWithAddressClaim( );
	uint16_t pseudoDelay = createPseudoDelay( acl.caName, acl.tickMs );

	// when
	updateAddressClaimed( &acl ); // transitioning to a delay handling state
	for ( uint16_t i = 0u; i < ( pseudoDelay + 1u ); i++ )
	{
		updateAddressClaimed( &acl ); // processing
	}
	updateAddressClaimed( &acl ); // one more for the address claim to go through

	// then
}

TEST( addressClaimed, given_a_contending_message_with_name_bigger_than_mine_when_in_wait_for_contention_state_then_address_reclaimed )
{
	// given
	uint8_t contenderName[ 8 ] = { 0xf1u, 0xf2u, 0xf3u, 0xf4u, 0xf5u, 0xf6u, 0xf7u, 0xf8u + 1u };
	acl.state = WAIT_FOR_CONTENTION;

	expectNoBusOff( 2u );
	registerReceivedContention( &acl, contenderName );
	expectOneCallToSendMessageWithTxBufferFull( );
	expectOneCallToSendMessageWithAddressClaim( );

	// when
	updateAddressClaimed( &acl );
	updateAddressClaimed( &acl );

	// then
}

TEST( addressClaimed, given_a_contending_message_with_name_equal_to_mine_when_in_wait_for_contention_state_then_address_changed_to_128 )
{
	// given
	uint8_t contenderName[ 8 ] = { 0xf1u, 0xf2u, 0xf3u, 0xf4u, 0xf5u, 0xf6u, 0xf7u, 0xf8u };
	acl.state = WAIT_FOR_CONTENTION;

	expectNoBusOff( 1u );
	registerReceivedContention( &acl, contenderName );
	expectAddressClaimWithAddress( 0x80u );

	// when
	updateAddressClaimed( &acl );

	// then
	UNSIGNED_LONGS_EQUAL( 128u, acl.sourceAddress );
}

TEST( addressClaimed, given_contention_and_no_available_address_when_prioritizing_contention_then_cannot_claim_message_sent )
{
	// given
	uint8_t contenderName[ 8 ] = { 0xf1u, 0xf2u, 0xf3u, 0xf4u, 0xf5u, 0xf6u, 0xf7u, 0xf8u };
	acl.state = WAIT_FOR_CONTENTION;

	expectNoAddressAvailableForClaim( contenderName );
	expectNoBusOff( 1u );
	registerReceivedContention( &acl, contenderName );
	expectCannotClaimMessge( );

	// when
	updateAddressClaimed( &acl );
	updateAddressClaimed( &acl );
	UNSIGNED_LONGS_EQUAL( CANNOT_CLAIM_ADDRESS, acl.state );
	updateAddressClaimed( &acl );
	UNSIGNED_LONGS_EQUAL( CANNOT_CLAIM_ADDRESS, acl.state );

	// then
	UNSIGNED_LONGS_EQUAL( 254u, acl.sourceAddress );
}


TEST( addressClaimed, given_a_request_for_address_claim_when_cannot_claim_address_then_a_pseudo_delay_is_applied_before_sending_a_cannot_claim_messge )
{
	// given
	acl.state = CANNOT_CLAIM_ADDRESS;
	registerRequestForAddressClaim( &acl );
	acl.sourceAddress = 254u;
	uint16_t pseudoDelay = createPseudoDelay( acl.caName, acl.tickMs );
	expectCannotClaimMessge( );

	// when
	updateAddressClaimed( &acl );
	UNSIGNED_LONGS_EQUAL( DELAY_BEFORE_CANNOT_CLAIM, acl.state );
	for ( uint16_t i = 0u; i < ( pseudoDelay + 1u ); i++ )
	{
		updateAddressClaimed( &acl ); 
	}
	updateAddressClaimed( &acl );
	UNSIGNED_LONGS_EQUAL( CANNOT_CLAIM_ADDRESS, acl.state );

	// then
	UNSIGNED_LONGS_EQUAL( 254u, acl.sourceAddress );
}

TEST( addressClaimed, given_a_request_for_address_claim_when_in_normal_traffic_state_then_address_claim_message_is_sent )
{
	// given
	acl.state = NORMAL_TRAFFIC;

	registerRequestForAddressClaim( &acl );
	expectOneCallToSendMessageWithAddressClaim( );

	// when
	updateAddressClaimed( &acl );

	// then
}

TEST( addressClaimed, given_a_request_for_address_claim_when_in_normal_traffic_state_and_tx_buffer_full_then_address_claim_message_is_sent_when_buffer_has_slot )
{
	// given
	acl.state = NORMAL_TRAFFIC;


	registerRequestForAddressClaim( &acl );
	expectOneCallToSendMessageWithTxBufferFull( );
	expectOneCallToSendMessageWithAddressClaim( );

	// when
	updateAddressClaimed( &acl );
	updateAddressClaimed( &acl );

	// then
}

TEST( addressClaimed, given_a_received_message_with_own_source_address_when_in_normal_traffic_state_then_address_claim_message_is_sent )
{
	// given
	acl.state = NORMAL_TRAFFIC;

	registerReceivedMessageWithOwnSA( &acl );
	expectOneCallToSendMessageWithAddressClaim( );

	// when
	updateAddressClaimed( &acl );

	// then
}

TEST( addressClaimed, given_no_contention_or_claim_or_message_with_own_SA_when_in_normal_traffic_state_then_nothing_happens )
{
	// given
	acl.state = NORMAL_TRAFFIC;

	expectNoCallToSendMessage( );

	// when
	updateAddressClaimed( &acl );
	updateAddressClaimed( &acl );
	updateAddressClaimed( &acl );
	updateAddressClaimed( &acl );
	updateAddressClaimed( &acl );
	updateAddressClaimed( &acl );

	// then
}

TEST( addressClaimed, given_a_contention_with_name_greater_than_mine_when_in_normal_traffic_state_then_address_is_reclaimed )
{
	// given
	uint8_t contenderName[ 8 ] = { 0xf1u, 0xf2u, 0xf3u, 0xf4u, 0xf5u, 0xf6u, 0xf7u, 0xf8u + 1u };
	acl.state = NORMAL_TRAFFIC;
	registerReceivedContention( &acl, contenderName );
	expectOneCallToSendMessageWithAddressClaim( );

	// when
	updateAddressClaimed( &acl );

	// then
}

TEST( addressClaimed, given_a_contention_with_name_equal_to_mine_when_in_normal_traffic_state_then_new_address_selected_and_claimed )
{
	// given
	acl.state = NORMAL_TRAFFIC;
	registerReceivedContention( &acl, acl.caName );

	expectAddressClaimWithAddress( 0x80u );

	// when
	updateAddressClaimed( &acl );

	// then
	UNSIGNED_LONGS_EQUAL( 0x80u, acl.sourceAddress );
}

