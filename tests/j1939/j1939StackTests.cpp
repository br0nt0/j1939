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
#include "j1939/j1939Stack.h"
}

TEST_GROUP( j1939Stack )
{
    void setup( void )
    {
    }
    void teardown( void )
    {
    }
};

TEST( j1939Stack, given_null_stack_when_sending_j1939_message_then_no_seg_fault )
{
    // given

    // when
    uint8_t result = sendJ1939Message( NULL, NULL );

    // then
    UNSIGNED_LONGS_EQUAL( 1u, result );
}

TEST( j1939Stack, given_a_stack_description_when_accessing_the_interface_then_it_is_returned )
{
    // given
    struct j1939Struct testStack = { NULL, "Test Stack" };

    // when

    // then
    STRCMP_EQUAL( "Test Stack", getJ1939StackType( &testStack ) );
}

TEST( j1939Stack, given_null_stack_when_destroying_the_stack_then_no_seg_fault )
{
    // given

    // when
    destroyJ1939Stack( NULL );

    // then
}
