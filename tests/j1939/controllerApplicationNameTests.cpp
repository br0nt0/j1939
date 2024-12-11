/*******************************************************************************
 * @file	controllerApplicationNameTests.cpp
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C"
{
#include "j1939/controllerApplicationName.h"
}

TEST_GROUP( controllerApplicationName )
{
    void setup( void )
    {
    }
    void teardown( void )
    {
    }
};

TEST( controllerApplicationName, given_no_name_when_getting_then_null_is_returned )
{
    // given
    setControllerapplicationName( NULL );

    // when
    const uint8_t* name = getControllerApplicationName( );

    // then
    CHECK_TRUE( NULL == name );

}

TEST( controllerApplicationName, given_a_name_when_getting_then_it_is_returned )
{
    // given
    uint8_t expectedName[ 8 ] = { 0xf1u, 0xf2u, 0xf3u, 0xf4u, 0xf5u, 0xf6u, 0xf7u, 0xf8u };
    setControllerapplicationName( expectedName );

    // when
    const uint8_t* name = getControllerApplicationName( );

    // then
    MEMCMP_EQUAL( expectedName, name, sizeof( expectedName ) );

}
