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
    void setup( void )
    {
        spyCANDriver = createCANDriverSpy( );
        acl = createAddressClaimed( spyCANDriver, 10u, caName, 0x11u );
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
    void expectOneCallToSendMessageWithAddressClaim( void )
	{
		canMessageStruct_t expectedCANMessage;
        expectedCANMessage.id = 0x18eeff00u + getSourceAddress( acl );
        expectedCANMessage.isExtended = true;
		expectedCANMessage.dlc = 8u;
        expectedCANMessage.data = getCAName( acl );
        expectCANOperational( );
		mock( "CANSpy" ).expectOneCall( "sendMessage" )
			.withPointerParameter( "base", spyCANDriver )
			.withUnsignedIntParameter( "id", expectedCANMessage.id )
			.withBoolParameter( "isExtended", expectedCANMessage.isExtended )
			.withUnsignedIntParameter( "dlc", expectedCANMessage.dlc )
			.withPointerParameter( "data", expectedCANMessage.data )
			.andReturnValue( CAN_TX_SUCCEEDED );
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


