/*******************************************************************************
 * @file	canRegisterIOMockTests.cpp
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C"
{
#include "mcu/pic32mz/can/canRegisterIO.h"
}

TEST_GROUP( canRegisterIOMock )
{
    canRegisters_t canRegisters[ CAN_NUMBER_OF_MODULES ];
    void setup( void )
    {
    }
    void teardown( void )
    {
        mock( "PIC32MZ_CAN" ).checkExpectations( );
		mock( ).clear( );
    }
};

TEST( canRegisterIOMock, given_the_can_module_1_when_getting_address_pointer_from_HW_then_full_register_structure_returned )
{
    // given
    canRegisters_t* expectedCANModule1Registers = &canRegisters[ 0 ];
    mock( "PIC32MZ_CAN" )
        .expectOneCall( "getPIC32MZCANModuleRegs" )
        .withParameter( "module", CAN1 )
        .andReturnValue( expectedCANModule1Registers );

    // when
    canRegisters_t* canModule1Regs = getCANModuleRegisters( CAN1 );

    // then
    POINTERS_EQUAL( expectedCANModule1Registers, canModule1Regs );
}

TEST( canRegisterIOMock, given_the_can_module_2_when_getting_address_pointer_from_HW_then_full_register_structure_returned )
{
    // given
    
    canRegisters_t* expectedCANModule1Registers = &canRegisters[ 1 ];
    mock( "PIC32MZ_CAN" )
        .expectOneCall( "getPIC32MZCANModuleRegs" )
        .withParameter( "module", CAN2 )
        .andReturnValue( expectedCANModule1Registers );

    // when
    canRegisters_t* canModule1Regs = getCANModuleRegisters( CAN2 );

    // then
    POINTERS_EQUAL( expectedCANModule1Registers, canModule1Regs );
}
