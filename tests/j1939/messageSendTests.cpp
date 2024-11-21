/*******************************************************************************
 * @file	messageSendTests.cpp
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "drivers/CANDriverSpy.h"

extern "C"
{
#include "j1939/messageSend.h"
}

TEST_GROUP( messageSend )
{
    canDriver_t spy;
    j1939MessageStruct_t message;
    void setup( void )
    {
        spy = createCANDriverSpy( );
    }
    void teardown( void )
    {
        destroyCANDriver( spy );
        mock( "CANSpy" ).checkExpectations( );
		mock( ).clear( );
    }
};

TEST( messageSend, given_a_non_operational_can_driver_when_sending_j1939_message_to_CAN_layer_then_status_is_can_driver_is_not_operational )
{
    // given
    mock( "CANSpy" ).expectOneCall( "isOperational" ).withPointerParameter( "base", spy ).andReturnValue( false );

    // when
    uint8_t status = sendJ1939MessageToCANDriver( &message, spy );

    // then
    UNSIGNED_LONGS_EQUAL( CAN_DRIVER_NOT_OPERATIONAL, status );
}



