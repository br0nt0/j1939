/*******************************************************************************
 * @file	messageReceiveTests.cpp
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "drivers/CANDriverSpy.h"
extern "C"
{
#include "j1939/messageReceive.h"
}

TEST_GROUP( messageReceive )
{
    canDriver_t spyCANDriver;
    void setup( void )
    {
        spyCANDriver = createCANDriverSpy( );
    }
    void teardown( void )
    {
        destroyCANDriver( spyCANDriver );
        mock( "CANSpy" ).checkExpectations( );
		mock( ).clear( );
    }
};

TEST( messageReceive, given_null_driver_when_receiving_a_j1939_message_then_message_is_null )
{
    // given

    // when
    j1939Message_t message = receiveJ1939MessageFromCANDriver( spyCANDriver );

    // then
    CHECK_TRUE( NULL == message );
}
