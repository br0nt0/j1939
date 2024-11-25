/*******************************************************************************
 * @file	j1939StackFakeTests.cpp
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C"
{
}

#include "drivers/CANDriverSpy.h"
#include "j1939StackFake.h"

TEST_GROUP( j1939StackFake )
{
    j1939_t fakeStack;
    canDriver_t spyCANDriver;
    void setup( void )
    {
        spyCANDriver = createCANDriverSpy( );
        fakeStack = createJ1939FakeStack( spyCANDriver );
    }
    void teardown( void )
    {
        destroyJ1939Stack( fakeStack );
        destroyCANDriver( spyCANDriver );
        mock( "CANSpy" ).checkExpectations( );
		mock( ).clear( );
    }
    void expectCANMessgeToSucceed( uint32_t id, bool_t isExtended, uint8_t dlc, uint8_t* data )
    {
        mock( "CANSpy" ).expectOneCall( "isOperational" )
            .withPointerParameter( "base", spyCANDriver )
            .andReturnValue( true );
        mock( "CANSpy" ).expectOneCall( "sendMessage" )
            .withPointerParameter( "base", spyCANDriver )
            .withUnsignedIntParameter( "id", id )
            .withBoolParameter( "isExtended", isExtended )
            .withUnsignedIntParameter( "dlc", dlc )
            .withPointerParameter( "data", data )
            .andReturnValue( CAN_TX_SUCCEEDED );
    }
};

TEST( j1939StackFake, given_fake_j1939_stack_then_create_and_destroy )
{
    // given

    // when

    // then
}

TEST( j1939StackFake, given_fake_j1939_stack_when_sending_a_message_then_CAN_driver_picks_it_up )
{
    // given
    uint8_t data[ 8 ] = { 11u, 22u, 33u, 44u, 55u, 66u, 77u,88u };
    j1939MessageStruct_t testMessage;
    testMessage.parameterGroupNumber = 0xfe44u;
    testMessage.priority = 7u;
    testMessage.sourceAddress = 0x0au;
    testMessage.destinationAddress = 0x0bu;
    testMessage.dataSize = 8u;
    testMessage.data = data;

    expectCANMessgeToSucceed( ( testMessage.priority << 26u ) + ( testMessage.parameterGroupNumber << 8u ) + ( testMessage.sourceAddress ),
                                true,
                                8u,
                                testMessage.data);
    // when
    uint8_t status = sendJ1939Message( fakeStack, &testMessage );

    // then
    UNSIGNED_LONGS_EQUAL( CAN_TX_SUCCEEDED, status );
}