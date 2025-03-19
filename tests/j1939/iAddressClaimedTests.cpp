/*******************************************************************************
 * @file	addressClaimedProcedureTests.cpp
 * @brief
 * @author	@br0nt0
 * @date	2025
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C"
{
#include "j1939/iAddressClaimed.h"
}

TEST_GROUP( iAddressClaimed )
{
    struct aclInterfaceStruct doNothing = {
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
    };
    struct aclStruct testACL =
    {
        &doNothing,
        "TestAcl"
    };

    void setup( void )
    {
    }
    void teardown( void )
    {
    }
};

TEST( iAddressClaimed, given_null_input_when_destroying_acl_then_no_seg_fault )
{
    // given

    // when
    destroyACL( NULL );

    // then
}

TEST( iAddressClaimed, given_null_input_when_updating_the_state_machine_then_no_seg_fault )
{
    // given

    // when
    updateACLStateMachine( NULL );

    // then
}

TEST( iAddressClaimed, given_null_input_when_checking_if_acl_was_successful_then_no_seg_fault_and_result_false )
{
    // given

    // when

    // then
    CHECK_FALSE( wasACLSuccessful( NULL ) );
}

TEST( iAddressClaimed, given_null_input_when_registering_a_request_for_address_claim_then_no_seg_fault )
{
    // given

    // when
    registerRequestForACL( NULL );

    // then
}

TEST( iAddressClaimed, given_null_input_when_registering_a_received_message_with_own_source_address_then_no_seg_fault )
{
    // given

    // when
    registerRcvMessageWithOwnSourceAddress( NULL );

    // then
}

TEST( iAddressClaimed, given_null_input_when_registering_a_received_contention_then_no_seg_fault )
{
    // given

    // when
    registerACLContention( NULL, NULL );

    // then
}

TEST( iAddressClaimed, given_null_input_when_setting_and_getting_the_CA_name_then_no_seg_fault )
{
    // given

    // when
    setCAName( NULL, NULL );

    // then
    CHECK_TRUE( NULL == getCAName( NULL ) );
}

TEST( iAddressClaimed, given_null_input_when_setting_and_getting_the_source_address_then_no_seg_fault )
{
    // given

    // when
    setSourceAddress( NULL, 1u );

    // then
    UNSIGNED_LONGS_EQUAL( 0xfeu, getSourceAddress( NULL ) );
}

TEST( iAddressClaimed, given_a_test_procedure_when_checking_accessors_then_it_is_returned )
{
    // given

    // when

    // then
    STRCMP_EQUAL( "TestAcl", getACLType( &testACL ) );
}

