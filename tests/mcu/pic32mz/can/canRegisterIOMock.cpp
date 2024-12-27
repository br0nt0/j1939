/*******************************************************************************
 * @file	canRegisterIOMock.cpp
 * @brief	
 * @author	@br0nt0
 * @date	2024
 ******************************************************************************/
#include "CppUTestExt/MockSupport.h"

extern "C"
{
#include "mcu/pic32mz/can/canRegisterIO.h"
}

canRegisters_t* getCANModuleRegisters( canModule_t module )
{
    return ( canRegisters_t* ) mock( "PIC32MZ_CAN" )
        .actualCall( "getPIC32MZCANModuleRegs" )
        .withParameter( "module", module )
		.returnValue( ).getPointerValue( );
}

