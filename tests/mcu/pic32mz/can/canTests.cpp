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
#include "mcu/pic32mz/can/canRegisterIO.h"
#include "string.h"
}


TEST_GROUP( can )
{
    canDriver_t driver;
    canRegisters_t canRegs;
    void setup( void )
    {
        driver = createPIC32MZCANDriverForModule( CAN1 );
        memset( ( void* ) &canRegs, 0, sizeof( canRegisters_t ) );
    }
    void teardown( void )
    {
        destroyCANDriver( driver );
        mock( "PIC32MZ_CAN" ).checkExpectations( );
		mock( ).clear( );
    }
    void expectOneCallToGetCANRegistersOfModule( canModule_t module )
    {
        mock( "PIC32MZ_CAN" )
        .expectOneCall( "getPIC32MZCANModuleRegs" )
        .withParameter( "module", module )
        .andReturnValue( &canRegs );
    }
};

TEST( can, given_a_pic32mz_can_driver_then_it_can_be_created_and_destroyed )
{
    // given

    // when

    // then
    STRCMP_EQUAL( "PIC32MZ", getCANDriverType( driver ) );
}

TEST( can, given_CAN1_when_creating_a_pic32mz_CAN_dirver_then_CAN_module_1_is_configured )
{
    // given

    // when

    // then
    UNSIGNED_LONGS_EQUAL( CAN1, getPIC32MZCANConfiguredModule( driver ) );
}

TEST( can, given_CAN2_when_creating_a_pic32mz_CAN_dirver_then_CAN_module_2_is_configured )
{
    // given
    canDriver_t can2Driver = createPIC32MZCANDriverForModule( CAN2 );

    // when

    // then
    UNSIGNED_LONGS_EQUAL( CAN2, getPIC32MZCANConfiguredModule( can2Driver ) );
    destroyCANDriver( can2Driver );
}

TEST( can, given_operational_module_when_checking_the_status_then_true_status_is_returned )
{
    // given
    canRegs.CxCON.bits.CANBUSY = 1u;
    expectOneCallToGetCANRegistersOfModule( CAN1 );

    // when

    // then
    CHECK_TRUE( isCANDriverOperational( driver ) );
}

TEST( can, given_non_operational_module_when_checking_the_status_then_false_status_is_returned )
{
    // given
    canRegs.CxCON.bits.CANBUSY = 0u;
    expectOneCallToGetCANRegistersOfModule( CAN1 );

    // when

    // then
    CHECK_FALSE( isCANDriverOperational( driver ) );
}