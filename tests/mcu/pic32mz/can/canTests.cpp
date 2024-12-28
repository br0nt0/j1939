/*******************************************************************************
 * @file	canTests.cpp
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C"
{
#include "mcu/pic32mz/can/can.h"
}

TEST_GROUP( can )
{
    canDriver_t driver;
    void setup( void )
    {
        driver = createPIC32MZCANDriver( );
    }
    void teardown( void )
    {
        destroyCANDriver( driver );
    }
};

TEST( can, given_a_pic32mz_can_driver_then_it_can_be_created_and_destroyed )
{
    // given

    // when

    // then
    STRCMP_EQUAL( "PIC32MZ", getCANDriverType( driver ) );
}

TEST( can, given_an_operational_can_driver_when_checking_the_status_then_status_is_returned )
{
    // given

    // when

    // then
    CHECK_TRUE( isCANDriverOperational( driver ) );
}