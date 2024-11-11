/*******************************************************************************
 * @file	fooTests.cpp
 * @brief
 * @author  @br0nt0
 * @date	2024
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C"
{
}

TEST_GROUP( foo )
{
    void setup( void )
    {
    }
    void teardown( void )
    {
    }
};

TEST( foo, given_null_then_no_seg_fault )
{
    // given

    // when

    // then
    CHECK_TRUE( true );
}
