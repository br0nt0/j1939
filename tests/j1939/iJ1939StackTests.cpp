/*******************************************************************************
 * @file	j1939StackTests.cpp
 * @brief
 * @author  @br0nt0
 * @date	2024
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C"
{
#include "j1939/iJ1939Stack.h"
}

TEST_GROUP( iJ1939Stack )
{
    void setup( void )
    {
    }
    void teardown( void )
    {
    }
};

TEST( iJ1939Stack, given_null_stack_when_sending_j1939_message_then_no_seg_fault )
{
    // given

    // when
    uint8_t result = sendJ1939Message( NULL, NULL );

    // then
    UNSIGNED_LONGS_EQUAL( 1u, result );
}

TEST( iJ1939Stack, given_a_stack_description_when_accessing_the_interface_then_it_is_returned )
{
    // given
    struct j1939Struct testStack = { NULL, "Test Stack" };

    // when

    // then
    STRCMP_EQUAL( "Test Stack", getJ1939StackType( &testStack ) );
}

TEST( iJ1939Stack, given_null_stack_when_destroying_the_stack_then_no_seg_fault )
{
    // given

    // when
    destroyJ1939Stack( NULL );

    // then
}

TEST( iJ1939Stack, given_null_stack_when_getting_stack_source_address_then_0xff_returned )
{
    // given

    // when

    // then
    UNSIGNED_LONGS_EQUAL( 0xffu, getJ1939SourceAddress( NULL ) );
}

TEST( iJ1939Stack, given_null_stack_when_setting_stack_source_address_then_noting_happens )
{
    // given

    // when
    setJ1939SourceAddress( NULL, 0x11u );

    // then
}

TEST( iJ1939Stack, given_null_stack_when_getting_stack_CA_name_then_null_returned )
{
    // given

    // when
    const uint8_t* caName = getJ1939CAName( NULL );

    // then
    CHECK_TRUE( NULL == caName );
}

TEST( iJ1939Stack, given_null_stack_when_setting_CA_name_then_noting_happens )
{
    // given
    uint8_t caName[ 8 ] = { 0x11u, 0x22u, 0x33u, 0x44u, 0x55u, 0x66u, 0x77u, 0x88u };

    // when
    setJ1939CAName( NULL, caName );

    // then
}

TEST( iJ1939Stack, given_null_stack_when_updating_core_then_nothing_happens )
{
    // given

    // when
    updateJ1939CoreScheduler( NULL );

    // then
}

TEST( iJ1939Stack, given_null_stack_when_checking_for_address_claimed_then_false_returned )
{
    // given

    // when

    // then
    CHECK_FALSE( wasJ1939AddressClaimed( NULL ) );
}