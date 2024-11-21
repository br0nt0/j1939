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
    }
};

TEST( j1939StackFake, given_fake_j1939_stack_then_create_and_destroy )
{
    // given

    // when

    // then
}
