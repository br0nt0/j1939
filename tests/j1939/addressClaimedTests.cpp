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
}

TEST_GROUP( addressClaimed )
{
    aclConfigStruct_t config;
    canDriver_t spyCanDriver;
    uint8_t caName[ 8 ] = { 0xf1u, 0xf2u, 0xf3u, 0xf4u, 0xf5u, 0xf6u, 0xf7u, 0xf8u };
    void setup( void )
    {
        spyCanDriver = createCANDriverSpy( );
        config.initialState = UNDEFINED;
        config.driver = spyCanDriver;
        config.sourceAddress = 0x01u;
        config.tickMs = 10u;
        config.caName = caName;
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
    void expectOneCallToSendMessageWithAddressClaim( aclConfigStruct_t* conf )
    {
        canMessageStruct_t expectedCANMessage;
        expectedCANMessage.id = 0x18eeff00u + conf->sourceAddress;
        expectedCANMessage.isExtended = true;
        expectedCANMessage.dlc = 8u;
        expectedCANMessage.data = conf->caName;
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
    void expectNoBusOff( uint8_t n )
	{
        mock( "CANSpy" ).expectNCalls( n, "isTxBusOffState" )
            .withPointerParameter( "base", spyCanDriver )
            .andReturnValue( false );
    }
};

TEST( addressClaimed, given_no_CAN_driver_when_configuring_address_claimed_procedure_then_no_address_claimed_messaged_sent_to_CAN_driver )
{
    // given
    config.driver = NULL;
    configureAddressClaimed( &config );
    expectNoCallToSendMessage( );

    // when
    updateAddressClaimed( );

    // then
}

TEST( addressClaimed, given_null_CA_name_when_configuring_address_claimed_procedure_then_no_address_claimed_messaged_sent_to_CAN_driver )
{
    // given
    config.caName = NULL;
    configureAddressClaimed( &config );
    expectNoCallToSendMessage( );

    // when
    updateAddressClaimed( );

    // then
}

TEST( addressClaimed, given_zero_ms_tick_when_configuring_address_claimed_procedure_then_no_address_claimed_messaged_sent_to_CAN_driver )
{
    // given
    config.tickMs = 0u;
    configureAddressClaimed( &config );
    expectNoCallToSendMessage( );

    // when
    updateAddressClaimed( );

    // then
}

TEST( addressClaimed, given_an_init_state_an_address_and_valid_config_when_updating_address_claimed_procedure_then_an_address_claimed_messaged_is_sent_to_CAN_driver )
{
    // given
    config.initialState = INIT;
    config.driver = spyCanDriver;
    config.sourceAddress = 0x01u;
    config.tickMs = 10u;
    config.caName = caName;
    configureAddressClaimed( &config );

    expectOneCallToSendMessageWithAddressClaim( &config );

    // when
    updateAddressClaimed( );

    // then
}

TEST( addressClaimed, given_no_address_content_and_no_bus_off_when_in_wait_for_contention_state_then_address_successfully_claimed_after_250ms )
{
    // given
    config.initialState = WAIT_FOR_CONTENTION;
    configureAddressClaimed( &config );
    expectNoBusOff( CONTENTION_TIMEOUT_MS / config.tickMs );   

    // when
    for ( size_t i = 0; i < ( CONTENTION_TIMEOUT_MS / config.tickMs ); i++ )
	{
		updateAddressClaimed( );
	}

    // then
    CHECK_TRUE( wasAddressClaimedSuccessfuly( ) );
}