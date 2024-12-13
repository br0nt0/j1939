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
    j1939_t testStack;
    uint8_t caName[ 8 ] = { 0xf1u, 0xf2u, 0xf3u, 0xf4u, 0xf5u, 0xf6u, 0xf7u, 0xf8u };
    int8_t initialState;
    void setup( void )
    {
        spyCanDriver = createCANDriverSpy( );
        testStack = createJ1939StackInstance( spyCanDriver, 10u );
        setJ1939CAName( testStack, caName );
        setJ1939SourceAddress( testStack, 0x01u );
        initialState = UNDEFINED;
    }
    void teardown( void )
    {
        destroyCANDriver( spyCanDriver );
        mock( "CANSpy" ).checkExpectations( );
        mock( ).clear( );
    }
    void expectNoCallToSendMessage( void )
    {
        mock( "CANSpy" ).expectNoCall( "isOperational" );
        mock( "CANSpy" ).expectNoCall( "sendMessage" );
    }
    void expectOneCallToSendMessageWithAddressClaim( void )
    {
        canMessageStruct_t expectedCANMessage;
        expectedCANMessage.id = 0x18eeff00u + getJ1939SourceAddress( testStack );
        expectedCANMessage.isExtended = true;
        expectedCANMessage.dlc = 8u;
        expectedCANMessage.data = getJ1939CAName( testStack );
        mock( "CANSpy" ).expectOneCall( "isOperational" )
            .withPointerParameter( "base", spyCanDriver )
            .andReturnValue( true );
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
        expectedCANMessage.id = 0x18eeff00u + getJ1939SourceAddress( testStack );
        expectedCANMessage.isExtended = true;
        expectedCANMessage.dlc = 8u;
        expectedCANMessage.data = getJ1939CAName( testStack );
        mock( "CANSpy" ).expectOneCall( "isOperational" )
            .withPointerParameter( "base", spyCanDriver )
            .andReturnValue( true );
        mock( "CANSpy" ).expectOneCall( "sendMessage" )
            .withPointerParameter( "base", spyCanDriver )
            .withUnsignedIntParameter( "id", expectedCANMessage.id )
            .withBoolParameter( "isExtended", expectedCANMessage.isExtended )
            .withUnsignedIntParameter( "dlc", expectedCANMessage.dlc )
            .withPointerParameter( "data", expectedCANMessage.data )
            .andReturnValue( CAN_TX_BUFFER_FULL );
    }
    void expectNoBusOff( uint8_t n )
    {
        mock( "CANSpy" ).expectNCalls( n, "isTxBusOffState" )
            .withPointerParameter( "base", spyCanDriver )
            .andReturnValue( false );
    }
    void expectNoAddressClaimMessgeToBeSent( void )
    {
        mock( "CANSpy" ).expectNoCall( "isOperational" );
        mock( "CANSpy" ).expectNoCall( "sendMessage" );
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

TEST( addressClaimed, given_null_stack_when_configuring_address_claimed_procedure_then_no_address_claimed_messaged_sent_to_CAN_driver )
{
    // given
    configureAddressClaimed( NULL, initialState );
    expectNoCallToSendMessage( );

    // when
    updateAddressClaimed( );

    // then
}

TEST( addressClaimed, given_an_address_and_valid_config_when_in_init_state_then_an_address_claimed_messaged_is_sent_to_CAN_driver )
{
    // given
    configureAddressClaimed( testStack, INIT );
    expectOneCallToSendMessageWithAddressClaim( );

    // when
    updateAddressClaimed( );

    // then
}

TEST( addressClaimed, given_a_failed_message_transmit_when_in_init_state_then_an_address_claimed_messaged_is_sent_to_CAN_driver_with_next_iteration )
{
    // given
    configureAddressClaimed( testStack, INIT );

    expectOneCallToSendMessageWithTxBufferFull( );
    expectOneCallToSendMessageWithAddressClaim( );

    // when
    updateAddressClaimed( );
    updateAddressClaimed( );

    // then
}

TEST( addressClaimed, given_no_address_content_and_no_bus_off_when_in_wait_for_contention_state_then_address_successfully_claimed_after_250ms )
{
    // given
    uint8_t configTicksMs = getJ1939ConfiguredTickMs( testStack );
    configureAddressClaimed( testStack, WAIT_FOR_CONTENTION );
    expectNoBusOff( CONTENTION_TIMEOUT_MS / configTicksMs );

    // when
    for ( size_t i = 0; i < ( CONTENTION_TIMEOUT_MS / configTicksMs ); i++ )
    {
        updateAddressClaimed( );
    }

    // then
    CHECK_TRUE( wasAddressClaimedSuccessfuly( ) );
}

TEST( addressClaimed, given_bus_off_event_when_in_wait_for_contention_state_then_a_random_delay_is_created_from_name_until_address_reclaim )
{
    // given
    configureAddressClaimed( testStack, WAIT_FOR_CONTENTION );

    mock( "CANSpy" ).expectOneCall( "isTxBusOffState" ).withPointerParameter( "base", spyCanDriver ).andReturnValue( true );
    expectOneCallToSendMessageWithAddressClaim( );
    uint16_t pseudoDelay = createPseudoDelay( getJ1939CAName( testStack ), getJ1939ConfiguredTickMs( testStack ) );

    // when
    updateAddressClaimed( ); // transition to a delay handling state
    for ( uint16_t i = 0u; i < ( pseudoDelay + 1u ); i++ )
    {
        updateAddressClaimed( ); // processing
    }

    // then
}

TEST( addressClaimed, given_bus_off_event_and_tx_buffer_full_when_in_wait_for_contention_state_then_a_random_delay_is_created_from_name_until_address_reclaim )
{
    // given
    configureAddressClaimed( testStack, WAIT_FOR_CONTENTION );

    mock( "CANSpy" ).expectOneCall( "isTxBusOffState" ).withPointerParameter( "base", spyCanDriver ).andReturnValue( true );
    expectOneCallToSendMessageWithTxBufferFull( );
    expectOneCallToSendMessageWithAddressClaim( );
    uint16_t pseudoDelay = createPseudoDelay( getJ1939CAName( testStack ), getJ1939ConfiguredTickMs( testStack ) );

    // when
    updateAddressClaimed( ); // transitioning to a delay handling state
    for ( uint16_t i = 0u; i < ( pseudoDelay + 1u ); i++ )
    {
        updateAddressClaimed( ); // processing
    }
    updateAddressClaimed( ); // one more for the address claim to go through

    // then
}

TEST( addressClaimed, given_a_contending_message_with_name_bigger_than_mine_when_in_wait_for_contention_state_then_address_reclaimed )
{
    // given
    uint8_t contenderName[ 8 ] = { 0xf1u, 0xf2u, 0xf3u, 0xf4u, 0xf5u, 0xf6u, 0xf7u, 0xf8u + 1u };
    configureAddressClaimed( testStack, WAIT_FOR_CONTENTION );

    expectNoBusOff( 2u );
    registerReceivedContention( contenderName );
    expectOneCallToSendMessageWithTxBufferFull( );
    expectOneCallToSendMessageWithAddressClaim( );

    // when
    updateAddressClaimed( );
    updateAddressClaimed( );

    // then
}

IGNORE_TEST( addressClaimed, given_a_contending_message_with_name_equal_to_mine_when_in_wait_for_contention_state_then_address_changed_to_128 )
{
    // given
    uint8_t contenderName[ 8 ] = { 0xf1u, 0xf2u, 0xf3u, 0xf4u, 0xf5u, 0xf6u, 0xf7u, 0xf8u };
    configureAddressClaimed( testStack, WAIT_FOR_CONTENTION );

    expectNoBusOff( 1u );
    registerReceivedContention( contenderName );
    expectOneCallToSendMessageWithAddressClaim( );

    // when
    updateAddressClaimed( );

    // then
}

TEST( addressClaimed, given_a_request_for_address_claim_when_in_normal_traffic_state_then_address_claim_message_is_sent )
{
    // given
    configureAddressClaimed( testStack, NORMAL_TRAFFIC );

    registerRequestForAddressClaim( );
    expectOneCallToSendMessageWithAddressClaim( );

    // when
    updateAddressClaimed( );

    // then
}

TEST( addressClaimed, given_a_request_for_address_claim_when_in_normal_traffic_state_and_tx_buffer_full_then_address_claim_message_is_sent_when_buffer_has_slot )
{
    // given
    configureAddressClaimed( testStack, NORMAL_TRAFFIC );


    registerRequestForAddressClaim( );
    expectOneCallToSendMessageWithTxBufferFull( );
    expectOneCallToSendMessageWithAddressClaim( );

    // when
    updateAddressClaimed( );
    updateAddressClaimed( );

    // then
}

TEST( addressClaimed, given_a_received_message_with_own_source_address_when_in_normal_traffic_state_then_address_claim_message_is_sent )
{
    // given
    configureAddressClaimed( testStack, NORMAL_TRAFFIC );

    registerReceivedMessageWithOwnSA( );
    expectOneCallToSendMessageWithAddressClaim( );

    // when
    updateAddressClaimed( );

    // then
}

TEST( addressClaimed, given_no_contention_or_claim_or_message_with_own_SA_when_in_normal_traffic_state_then_nothing_happens )
{
    // given
    configureAddressClaimed( testStack, NORMAL_TRAFFIC );

    expectNoAddressClaimMessgeToBeSent( );

    // when
    updateAddressClaimed( );
    updateAddressClaimed( );
    updateAddressClaimed( );
    updateAddressClaimed( );
    updateAddressClaimed( );
    updateAddressClaimed( );

    // then
}
