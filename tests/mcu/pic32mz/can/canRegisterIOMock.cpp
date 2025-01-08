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

canRxMessageBuffer_t* getRxCxFIFOUA( canModule_t module, canFifo_t fifo )
{
    return ( canRxMessageBuffer_t* ) mock( "PIC32MZ_CAN" )
        .actualCall( "getPIC32MZRxCxFIFOUA" )
        .withParameter( "module", module )
        .withParameter( "fifo", fifo )
        .returnValue( ).getPointerValue( );
}

canTxMessageBuffer_t* getTxCxFIFOUA( canModule_t module, canFifo_t fifo )
{
    return ( canTxMessageBuffer_t* ) mock( "PIC32MZ_CAN" )
        .actualCall( "getPIC32MZTxCxFIFOUA" )
        .withParameter( "module", module )
        .withParameter( "fifo", fifo )
        .returnValue( ).getPointerValue( );
}


